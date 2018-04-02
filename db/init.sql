CREATE TABLE IF NOT EXISTS algoritmos (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	nome CHAR(50));

CREATE TABLE IF NOT EXISTS comparadores (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	nome CHAR(50));

CREATE TABLE IF NOT EXISTS distorcoes (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	descricao CHAR(50),
	slug CHAR(50));

CREATE TABLE IF NOT EXISTS videos (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	filename CHAR(200),
	distorcao_id INTEGER NULL,
	video_original INTEGER NULL, 
	FOREIGN KEY(distorcao_id) REFERENCES distorcoes(id),
	FOREIGN KEY(video_original) REFERENCES video(id));

CREATE TABLE IF NOT EXISTS assinaturas (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	id_video INTEGER NOT NULL,
	id_algoritmo INTEGER,
	filename CHAR(200),
	FOREIGN	KEY(id_video) REFERENCES videos(id),
	FOREIGN	KEY(id_algoritmo) REFERENCES algoritmos(id));

CREATE TABLE IF NOT EXISTS testes (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	id_video_original INTEGER,
	id_video_teste INTEGER,
	id_comparador INTEGER,
	data_criacao DATETIME,
	resultado_bruto REAL,
	resultado_copia BOOLEAN,
	FOREIGN	KEY(id_video_original) REFERENCES videos(id),
	FOREIGN	KEY(id_video_teste) REFERENCES videos(id),
	FOREIGN	KEY(id_comparador) REFERENCES comparadores(id));


# ------------------------------------
INSERT INTO algoritmos (`nome`) VALUES ('Gradiente'), ('FrameDiff'), ('Medida Ordinal'), ('Wavelets'), ('RBP'), ('Scene Frame');
INSERT INTO distorcoes ()