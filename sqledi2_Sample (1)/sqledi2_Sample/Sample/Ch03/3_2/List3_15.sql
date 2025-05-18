SELECT shiire_tanka, COUNT(*)
  FROM Shohin
 WHERE shohin_bunrui = '衣服'
 GROUP BY shiire_tanka;