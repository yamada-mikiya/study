/*
  以下は
  問のSELECT文
*/
SELECT shohin_id, SUM(shohin_mei)
-- この SELECT 文は間違っています。
  FROM Shohin
 GROUP BY shohin_bunrui
 WHERE torokubi > '2009-09-01';


/* 答えのSELECT 文の一例
   間違いの箇所は以下の通り：
     1. WHERE句はGROUP BY句の前に来る必要があります。
     2. GROUP BY句を使う場合、SELECT句に書ける列は、GROUP BY句で集約キーになっている列（shohin_bunrui）だけです。
     3. SUM関数の引数に取れるのは数値列のみです。
*/
SELECT shohin_bunrui, SUM(hanbai_tanka)
  FROM Shohin
 WHERE torokubi > '2009-09-01'
 GROUP BY shohin_bunrui;

