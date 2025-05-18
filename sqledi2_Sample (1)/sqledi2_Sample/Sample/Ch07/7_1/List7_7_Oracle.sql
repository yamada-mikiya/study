--OracleではEXCEPTではなくMINUSを使う
SELECT shohin_id, shohin_mei
  FROM Shohin
MINUS
SELECT shohin_id, shohin_mei
  FROM Shohin2
ORDER BY shohin_id;