SELECT
	t.id_video_original v1_id,
	t.id_video_teste v2_id,
	v1.video_original v1_original,
	v2.video_original v2_original,
	CASE
	WHEN v1.id = v2.id THEN 'mesmo_video'
	WHEN (v1.id = v2.video_original) OR (v1.id = v2.video_original) THEN 'original_distorcido'
	WHEN (NOT ((v1.id = v2.video_original) OR (v1.id = v2.video_original))) AND v1.video_original = v2.video_original THEN 'distorcido_distorcido'
	ELSE 'videos_diferentes' 
	END status,
	resultado_bruto
FROM
	testes t 
	inner join 
	videos v1 ON t.id_video_original = v1.id
	inner join 
	videos v2 ON t.id_video_teste = v2.id
