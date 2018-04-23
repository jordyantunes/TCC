# Inserir vídeos no banco de dados

```bash
cd ~/GitHub/TCC/db
python get_videos.py 
```

usage: get_files.py [-h] [-p [PATH]] [-e EXTENSOES [EXTENSOES ...]] [-d]
                    [-db DATABASE] [-dp [DATABASE_PATH]]

optional arguments:
  -h, --help            show this help message and exit
  -p [PATH], --path [PATH]
                        Caminho dos arquivos de vídeo
  -e EXTENSOES [EXTENSOES ...], --extensoes EXTENSOES [EXTENSOES ...]
                        Extensões adicionais (padrão: .avi, .mp4)
  -d, --debug           Ativar debug
  -db DATABASE, --database DATABASE
                        Nome do arquivo de banco de dados sqlite
  -dp [DATABASE_PATH], --database-path [DATABASE_PATH]
                        Caminho da base de dados



# Criar link simbólico para videos (busca recursivamente numa pasta)

```bash
cd ~/GitHub/TCC/
python copy_videos_to_single_folder.py -op 
```


usage: copy_videos_to_single_folder.py [-h] [-op ORIGIN_PATH]
                                       [-dp DESTINATION_PATH] [-sl]

optional arguments:
  -h, --help            show this help message and exit
  -op ORIGIN_PATH, --origin-path ORIGIN_PATH
  -dp DESTINATION_PATH, --destination-path DESTINATION_PATH
  -sl, --symlink        Criar link simbolico ao inves de copiar o video


# Gerar assinaturas
```bash
cd ~/GitHub/TCC/src
python gerarassinaturas.py ...
```


usage: gerarassinaturas.py [-h] [-vp VIDEO_PATH] [-fp FINGERPRINT_PATH]
                           [-df DATABASE_FILENAME]

optional arguments:
  -h, --help            show this help message and exit
  -vp VIDEO_PATH, --video-path VIDEO_PATH
                        Caminho dos vídeos
  -fp FINGERPRINT_PATH, --fingerprint-path FINGERPRINT_PATH
                        Caminho das assinaturas
  -df DATABASE_FILENAME, --database-file DATABASE_FILENAME
                        Caminho do banco de dados


# Testar assinaturas
```bash
cd ~/GitHub/TCC/src
python testeassinaturas.py ...
```

usage: testeassinaturas.py [-h] [-vp VIDEO_PATH] [-fp FINGERPRINT_PATH]
                           [-df DATABASE_FILENAME]

optional arguments:
  -h, --help            show this help message and exit
  -vp VIDEO_PATH, --video-path VIDEO_PATH
                        Caminho dos vídeos
  -fp FINGERPRINT_PATH, --fingerprint-path FINGERPRINT_PATH
                        Caminho das assinaturas
  -df DATABASE_FILENAME, --database-file DATABASE_FILENAME
                        Caminho do banco de dados