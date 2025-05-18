SELECT shohin_bunrui, COUNT(*)
  FROM Shohin
 WHERE shohin_bunrui = '衣服'
 GROUP BY shohin_bunrui;