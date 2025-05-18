SELECT SUM(CASE WHEN hanbai_tanka <= 1000 THEN 1 ELSE 0 END)               AS low_price,
       SUM(CASE WHEN hanbai_tanka BETWEEN 1001 AND 3000 THEN 1 ELSE 0 END) AS mid_price,
       SUM(CASE WHEN hanbai_tanka >= 3001 THEN 1 ELSE 0 END)               AS high_price
 FROM Shohin;

 
 select count(case when hanbai_tanka <= 1000 then shohin_id else null end) as low_price,
        count(case when hanbai_tanka between 1001 and 3000 then shohin_id else null end) as mid_price,
        count(case when hanbai_tanka >= 3001 then shohin_id else null end) as mid_price
        from Shohin;