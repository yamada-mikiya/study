package main

import (
	"net/http"

	"example.com/jwt-auth/auth"
	"github.com/labstack/echo/v4"
)

type post struct {
	Title string `json:"title"`
	Tag   string `json:"tag"`
	URL   string `json:"url"`
}

func main() {
	e := echo.New()

	// localhost:8080/public で publicHandler を実行
	e.GET("/public", publicHandler)

	// /auth は認証なしでアクセス可能（トークン発行）
	e.GET("/auth", auth.GetTokenHandler)

	// /private はJWTミドルウェアで保護されている
	privateGroup := e.Group("/private")
	privateGroup.Use(auth.JwtMiddleware())
	privateGroup.GET("", privateHandler)

	// サーバー起動
	e.Logger.Fatal(e.Start(":8080"))

}

func publicHandler(c echo.Context) error {
	p := &post{
		Title: "VueCLIからVue.js入門①【VueCLIで出てくるファイルを概要図で理解】",
		Tag:   "Vue.js",
		URL:   "https://qiita.com/po3rin/items/3968f825f3c86f9c4e21",
	}
	return c.JSON(http.StatusOK, p)
}

func privateHandler(c echo.Context) error {
	p := &post{
		Title: "VGolangとGoogle Cloud Vision APIで画像から文字認識するCLIを速攻でつくる",
		Tag:   "Go",
		URL:   "https://qiita.com/po3rin/items/bf439424e38757c1e69b",
	}
	return c.JSON(http.StatusOK, p)
}
