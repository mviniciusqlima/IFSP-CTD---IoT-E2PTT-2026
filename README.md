# IFSP-CTD---IoT-E2PTT-2026
Projeto desenvolvido na disciplina de Plataformas de Prototipação para IoT

# Materiais utilizados

🔌 Hardware
-> ESP32
-> Módulo GPS NEO-6M
-> Sensor de obstáculo (infravermelho)
-> Display OLED (I2C)
-> Buzzer (ativo/passivo)
-> LEDs (indicadores visuais)
-> Botões (controle do sistema)
-> Protoboard
-> Jumpers (cabos de conexão)
-> Suporte para pilhas AAA

💻 Software e Tecnologias
-> Amazon Web Services (AWS)
-> Ubuntu (Servidor Linux)
-> Node-RED
--> Node-RED Dashboard
--> Node-RED Worldmap
-> MySQL
-> Apache HTTP Server
-> MQTT (Mosquitto)
-> Arduino IDE

# A proposta de Projeto

O projeto consiste em um sistema IoT baseado no ESP32, capaz de monitorar a presença de obstáculos e coletar coordenadas geográficas por meio do módulo GPS NEO-6M. Quando ativado, o dispositivo detecta obstáculos utilizando um sensor infravermelho e, ao identificar uma ocorrência, aciona alertas locais (LED e buzzer) e realiza o envio das coordenadas para a nuvem. O sistema também permite coleta manual de dados por meio de botões físicos.

Os dados são transmitidos via protocolo MQTT para um servidor hospedado na Amazon Web Services, onde são processados pelo Node-RED. Nesse ambiente, as informações recebidas são tratadas e armazenadas em um banco de dados MySQL, permitindo o registro histórico dos eventos e coordenadas coletadas pelo dispositivo.

Por fim, os dados armazenados são apresentados em um dashboard interativo, desenvolvido com Node-RED Dashboard, incluindo gráficos, tabela de eventos e visualização em mapa com o Node-RED Worldmap. Essa interface possibilita o monitoramento em tempo real do sistema, facilitando a análise das informações e demonstrando a integração completa entre hardware, comunicação em nuvem e visualização de dados.

# DashBoard

http://18.191.112.34:1880/dashboard/

# Resumo executivo

[ProjetoObstaculo_PrototipaçãoIoT_v20260316.pdf](https://github.com/user-attachments/files/26153985/ProjetoObstaculo_PrototipacaoIoT_v20260316.pdf)

# Fotos do Projeto

<img width="1280" height="963" alt="image" src="https://github.com/user-attachments/assets/6f258024-8fa2-4d52-9aff-a4eb9d5299d0" />

<img width="1280" height="963" alt="image" src="https://github.com/user-attachments/assets/55022b30-592a-4de6-81bc-0e8160787f95" />
