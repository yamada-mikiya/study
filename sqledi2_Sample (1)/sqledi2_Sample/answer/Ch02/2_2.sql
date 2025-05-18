/*
  以下は
  問のSELECT文
*/
-- ①
SELECT *
      FROM Shohin
     WHERE shiire_tanka = NULL;

-- ②
SELECT *
      FROM Shohin
     WHERE shiire_tanka <> NULL;

-- ③
SELECT *
      FROM Shohin
     WHERE shohin_mei > NULL;