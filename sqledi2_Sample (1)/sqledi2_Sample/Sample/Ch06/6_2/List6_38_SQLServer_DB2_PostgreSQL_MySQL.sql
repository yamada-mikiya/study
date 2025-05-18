SELECT shohin_mei, hanbai_tanka
  FROM Shohin AS S
 WHERE EXISTS (SELECT *
                 FROM TenpoShohin AS TS
                WHERE TS.tenpo_id = '000C'
                  AND TS.shohin_id = S.shohin_id);

select shohin_mei, CASE WHEN shohin_bunrui = '衣服'THEN 'A:' || shohin_bunrui
                    when shohin_bunrui ='事務用品'
                    then 'B:' || shohin_bunrui
                    when shohin_bunrui = 'キッチン用品'
                    then 'C:' || shohin_bunrui
                    else NULL
                END as abc_shohin_bunrui
from Shohin;
