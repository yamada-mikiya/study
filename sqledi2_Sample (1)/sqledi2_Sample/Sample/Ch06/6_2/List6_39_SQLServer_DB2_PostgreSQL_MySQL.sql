SELECT shohin_mei, hanbai_tanka
  FROM Shohin AS S
 WHERE EXISTS (SELECT 1 -- ここは適当な定数を書いてもかまいません。
                 FROM TenpoShohin AS TS
                WHERE TS.tenpo_id = '000C'
                  AND TS.shohin_id = S.shohin_id);