CREATE TABLE Shohin
(shohin_id     CHAR(4) NOT NULL,
 shohin_mei    VARCHAR(100) NOT NULL,
 shohin_bunrui VARCHAR(32) NOT NULL,
 hanbai_tanka  INTEGER ,
 shiire_tanka  INTEGER ,
 torokubi      DATE ,
 PRIMARY KEY (shohin_id));

 select count(case when hanbai_tanka <= 1000 then shohin_id else null end) as low_price,
        count(case when hanbai_tanka between 1001 and 3000 then shohin_id else null end) as mid_price,
        count(case when hanbai_tanka >= 3001 then shohin_id else null end) as mid_price
        from Shohin;