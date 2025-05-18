package auth

import (
	"fmt"
	"net/http"
	"time"

	"github.com/golang-jwt/jwt/v5" // ← v4 から v5 に変更
	echojwt "github.com/labstack/echo-jwt/v4"
	"github.com/labstack/echo/v4"
)

var signingKey = []byte("mysecretkey123")

// カスタムClaims構造体を定義する (推奨)
type JwtCustomClaims struct {
	Name  string `json:"name"`
	Admin bool   `json:"admin"`
	Sub   string `json:"sub"`
	jwt.RegisteredClaims // ← これで iat, exp などを標準で扱える
}


func GetTokenHandler(c echo.Context) error {
	// カスタムClaimsを使用
	claims := &JwtCustomClaims{
		"taro",
		true,
		"54546557354",
		jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(24 * time.Hour)),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
		},
	}

	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims) // ← NewWithClaims を使用

	tokenString, err := token.SignedString(signingKey)
	if err != nil {
		return echo.NewHTTPError(http.StatusInternalServerError, "Token generation failed")
	}

	return c.JSON(http.StatusOK, map[string]string{"token": tokenString})
}

// auth/auth.go
// ... (imports) ...

func JwtMiddleware() echo.MiddlewareFunc {
    fmt.Println("JwtMiddleware constructor called") // デバッグ用
    config := echojwt.Config{
        SigningKey:  signingKey,
        // TokenLookup: "header:Authorization,query:token,cookie:jwt", // queryやcookieも試す場合
        NewClaimsFunc: func(c echo.Context) jwt.Claims {
            return new(JwtCustomClaims)
        },
        SuccessHandler: func(c echo.Context) {
            fmt.Println("JWT Auth Successful for path:", c.Path())
            // 認証成功時は特に何もしなくても次のハンドラに進む
            // ユーザー情報をコンテキストにセットするなどはここで行える
            // 例:
            // user := c.Get("user").(*jwt.Token)
            // claims := user.Claims.(*JwtCustomClaims)
            // fmt.Println("User Name from token:", claims.Name)
        },
        ErrorHandler: func(c echo.Context, err error) error {
            fmt.Println("JWT Auth Error for path:", c.Path(), "Error:", err.Error())
            return echo.NewHTTPError(http.StatusUnauthorized, "invalid or expired jwt: "+err.Error())
        },
    }
    return echojwt.WithConfig(config)
}