/*
  以下は
  問のSELECT文
*/
-- ①
SELECT shohin_mei, shiire_tanka
  FROM Shohin
 WHERE shiire_tanka NOT IN (500, 2800, 5000);

-- ②
SELECT shohin_mei, shiire_tanka
  FROM Shohin
 WHERE shiire_tanka NOT IN (500, 2800, 5000, NULL);

