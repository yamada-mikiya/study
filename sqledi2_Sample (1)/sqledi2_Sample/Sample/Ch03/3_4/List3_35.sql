-- 列名で指定
SELECT shohin_id, shohin_mei, hanbai_tanka, shiire_tanka
  FROM Shohin
 ORDER BY hanbai_tanka DESC, shohin_id;

-- 列番号で指定
SELECT shohin_id, shohin_mei, hanbai_tanka, shiire_tanka
  FROM Shohin
 ORDER BY 3 DESC, 1;