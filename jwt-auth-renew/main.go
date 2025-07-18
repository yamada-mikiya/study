package main

import (
	"fmt"
	"net/http"
	"time"

	// jwtライブラリを新しいものに変更
	"github.com/golang-jwt/jwt/v5"
	// echo-jwtミドルウェアをインポート
	echojwt "github.com/labstack/echo-jwt/v4"
	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware" // Logger, Recover用
)

// このキーは実際にはもっと安全な方法で管理してください
var jwtSecret = []byte("VERY_SECRET_KEY_REPLACE_IT") // より安全なキーに変更推奨

// カスタムクレームの構造体
// jwt.RegisteredClaims を埋め込むことで、標準的なクレーム (exp, iatなど) を扱える
type JwtCustomClaims struct {
	UserID int64  `json:"user_id"`
	Name   string `json:"name"`
	jwt.RegisteredClaims
}

// ログイン処理＆トークン生成
func login(c echo.Context) error {
	username := c.FormValue("username")
	password := c.FormValue("password")

	if username != "testname" || password != "testpw" {
		return echo.ErrUnauthorized
	}

	// カスタムクレームを設定
	claims := &JwtCustomClaims{
		12345678,
		username,
		jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(time.Hour * 72)), // 有効期限を72時間に延長
			IssuedAt:  jwt.NewNumericDate(time.Now()),
			NotBefore: jwt.NewNumericDate(time.Now()),
			Issuer:    "my-auth-server",                         // 発行者
			Subject:   "user-login",                             // トークンの主題
			ID:        fmt.Sprintf("%d", time.Now().UnixNano()), // 一意なID
			Audience:  jwt.ClaimStrings{"my-app"},               // 想定される受信者
		},
	}

	// トークンオブジェクトを作成
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)

	// トークンに署名を付与
	tokenString, err := token.SignedString(jwtSecret)
	if err != nil {
		return fmt.Errorf("failed to sign token: %w", err)
	}

	return c.JSON(http.StatusOK, echo.Map{
		"token": tokenString,
	})
}

// ユーザ情報取得 (要認証)
func getUserInfo(c echo.Context) error {
	// ミドルウェアから渡されたJWTトークンを取得 (ContextKeyはデフォルトで "user")
	userToken := c.Get("user").(*jwt.Token)
	// カスタムクレームに型アサーション
	claims := userToken.Claims.(*JwtCustomClaims)

	userID := claims.UserID
	name := claims.Name
	issuer := claims.Issuer

	return c.String(http.StatusOK, fmt.Sprintf("Your UserID: %d, Name: %s, Token Issuer: %s", userID, name, issuer))
}

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.POST("/login", login)

	// 認証が必要なグループ
	restrictedGroup := e.Group("/user")

	// echo-jwtミドルウェアの設定
	// echojwt.Config 構造体を使用する
	config := echojwt.Config{
		NewClaimsFunc: func(c echo.Context) jwt.Claims {
			// この関数で、パース時に使用するクレームの型を指定する
			return new(JwtCustomClaims)
		},
		SigningKey: jwtSecret,
		// TokenLookup: "header:Authorization:Bearer <token>", // デフォルト値。変更可能
		// ContextKey: "user", // デフォルト値。変更可能
		// ErrorHandler: func(c echo.Context, err error) error { ... } // カスタムエラーハンドリング
	}

	// グループにミドルウェアを適用
	// JWTミドルウェアは echojwt.WithConfig を使用する
	restrictedGroup.Use(echojwt.WithConfig(config))

	restrictedGroup.GET("/info", getUserInfo)

	e.Logger.Info("Starting server on :8080")
	e.Logger.Fatal(e.Start(":8080"))
}
