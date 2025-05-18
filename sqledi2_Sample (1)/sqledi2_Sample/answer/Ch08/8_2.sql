-- 【①torokubiがNULLの場合、「1年1月1日」と見なす】
SELECT torokubi, shohin_mei, hanbai_tanka,
       SUM (hanbai_tanka) OVER (ORDER BY COALESCE(torokubi, CAST('0001-01-01' AS DATE))) AS current_sum_tanka
  FROM Shohin;

-- 【②torokubiがNULLの場合、先頭に持ってくるよう指定する】
SELECT torokubi, shohin_mei, hanbai_tanka,
       SUM (hanbai_tanka) OVER (ORDER BY torokubi NULLS FIRST) AS current_sum_tanka
  FROM Shohin;