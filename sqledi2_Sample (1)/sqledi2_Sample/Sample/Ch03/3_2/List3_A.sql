-- DISTINCTを使う場合
SELECT DISTINCT shohin_bunrui
  FROM Shohin;

-- GROUP BYを使う場合
SELECT shohin_bunrui
  FROM Shohin
 GROUP BY shohin_bunrui;