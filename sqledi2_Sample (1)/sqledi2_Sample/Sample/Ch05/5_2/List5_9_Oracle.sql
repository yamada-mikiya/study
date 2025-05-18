--Oracle
--FROM句に直接ビュー定義のSELECT文を書く
SELECT shohin_bunrui, cnt_shohin
  FROM (SELECT shohin_bunrui, COUNT(*) AS cnt_shohin
          FROM Shohin
         GROUP BY shohin_bunrui) ShohinSum;