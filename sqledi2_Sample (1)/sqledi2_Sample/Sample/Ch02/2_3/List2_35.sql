SELECT shohin_mei, shohin_bunrui, torokubi
  FROM Shohin
 WHERE shohin_bunrui = '事務用品'
   AND torokubi = '2009-09-11'
    OR torokubi = '2009-09-20';