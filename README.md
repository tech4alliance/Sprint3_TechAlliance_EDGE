# Tech Alliance - Sistema de Monitoramento IoT para Fórmula E 🏎️⚡

## Visão Geral do Projeto 🌍
O projeto Tech Alliance implementa uma solução de Internet das Coisas (IoT) voltada para o monitoramento em tempo real de veículos de corrida elétricos da Fórmula E. O sistema coleta dados importantes como temperatura dos componentes, umidade, localização GPS e velocidade dos veículos durante as corridas. Esses dados são visualizados em um painel interativo em tempo real, permitindo que as equipes tomem decisões mais rápidas e precisas para melhorar o desempenho e a segurança dos carros de corrida.

### Integrantes do Grupo 👥
Matheus Henriques do Amaral - 556957
Bruno Carneiro Leão - 555563
Guilherme Melo - 555310
Paulo Akira Okama - 556840
Victor Capp - 555753


## Arquitetura do Sistema 🏗️

### A arquitetura do sistema é dividida em duas camadas principais:

Camada IoT 🚗: Nesta camada, sensores e microcontroladores instalados nos carros de Fórmula E capturam dados críticos durante a corrida. Esses dados são transmitidos via Wi-Fi e MQTT para o servidor da equipe.

Microcontroladores: Dispositivos como ESP32 são usados para processar dados de sensores e transmiti-los em tempo real.
Sensores: Responsáveis por capturar informações como temperatura da bateria, velocidade e localização GPS.
Protocolo MQTT: Utilizado para garantir a transmissão eficiente dos dados para a nuvem ou servidores locais.

Camada de Aplicação 📊: Responsável por exibir os dados em um painel (dashboard) que a equipe técnica utiliza para monitorar as condições dos veículos e otimizar a estratégia de corrida.

Dash e Plotly: Bibliotecas utilizadas para criar gráficos interativos que exibem o estado do carro em tempo real.
STH-Comet: Para armazenar e consultar dados históricos, permitindo análises detalhadas após a corrida.

## Recursos Necessários 🚀

### Dispositivos IoT
Microcontroladores: ESP32 ou equivalentes para processar os dados dos sensores em tempo real.
Sensores:
Sensores de Temperatura: Para monitorar o aquecimento de componentes críticos, como a bateria e os freios.
Sensores de Umidade: Para verificar as condições atmosféricas que podem afetar o desempenho dos carros.
GPS: Para rastreamento da localização exata dos carros na pista.
Sensores de Velocidade: Para monitorar a performance dos veículos durante a corrida.

### Back-End ⚙️
FIWARE Orion Context Broker: Gerencia e armazena os dados de telemetria dos carros em tempo real.
STH-Comet: Armazena os dados históricos e permite consultas sobre o desempenho dos carros em corridas anteriores.
Broker MQTT: Facilita a comunicação entre os sensores IoT e o servidor central.

### Front-End 🎨
Dash (Python): Criação do painel interativo para visualização dos dados.
Plotly: Gráficos dinâmicos que mostram os dados dos carros.
HTML/CSS: Estruturação e estilização do dashboard.

## Passo a Passo de Implementação 🔧
1. Configuração do Microcontrolador (ESP32)
Conecte os sensores aos microcontroladores para monitorar temperatura, umidade, velocidade e localização GPS dos veículos.
Suba o código no ESP32 para que ele colete os dados dos sensores e envie-os para o servidor via MQTT.
2. Configuração do Back-End
Instale o FIWARE Orion Context Broker para gerenciar os dados em tempo real.
Configure o STH-Comet para armazenar o histórico de dados.
Configurar o broker MQTT (como Mosquitto) para garantir que os dados dos sensores cheguem ao servidor da equipe em tempo real.
3. Configuração do Front-End
Clone o repositório:
git clone https://github.com/seu-usuario/tech-alliance.git

Instale as dependências do Python:
pip install -r requirements.txt

Execute o painel de controle:
python app.py

Acesse o dashboard no navegador:
http://localhost:8051

## Explicação do Código 🧑‍💻
Códigos IoT (ESP32)
O código do ESP32 coleta dados dos sensores em tempo real e os envia para o broker MQTT. Aqui estão alguns detalhes do funcionamento do código:

### Leitura de Sensores:
O código lê os dados dos sensores de temperatura, umidade, velocidade e GPS e os armazena para serem enviados ao servidor.
Exemplo:
float temperature = dht.readTemperature();
MQTT.publish(TOPICO_PUBLISH_2, String(temperature).c_str());

### Transmissão de Dados via MQTT:
Após ler os sensores, o ESP32 utiliza o protocolo MQTT para enviar os dados para o servidor de telemetria.
Exemplo:
MQTT.publish(TOPICO_PUBLISH_5, String(latitude, 8).c_str());
MQTT.publish(TOPICO_PUBLISH_6, String(velocidadeGPS.valor).c_str());

### Códigos Front-End (Dashboard em Dash e Plotly)
O painel de controle foi construído utilizando Dash e Plotly para fornecer uma interface gráfica interativa e em tempo real. As principais funcionalidades incluem:

### Gráficos de Monitoramento em Tempo Real:

O sistema exibe gráficos de temperatura, velocidade e umidade em tempo real.
Exemplo:
trace_temp = go.Scatter(
  x=timestamps,
  y=temperature,
  mode='lines+markers',
  name='Temperatura'
)

O painel também exibe latitude, longitude e velocidade em cartões dinâmicos, atualizados em tempo real.
Exemplo:
html.H3(id='latitude', style={'color': 'white'}),
Histórico de Desempenho:

A equipe pode verificar o histórico de dados coletados durante a corrida para ajustes estratégicos.

## Funcionalidades Principais 🏆
Monitoramento em Tempo Real: Exibe a localização, velocidade, temperatura e umidade dos carros de Fórmula E durante a corrida.
Armazenamento e Consulta de Dados Históricos: O histórico de desempenho é armazenado e pode ser consultado para análises futuras.
Alertas de Desempenho: Possibilidade de criar alertas quando parâmetros críticos, como temperatura ou velocidade, atingem limites que podem comprometer a performance do carro.

## Instruções de Uso 📋
Inicie o sistema conectando os sensores ao ESP32 em cada veículo.
Monitore os dados em tempo real através do painel de controle, ajustando a estratégia de corrida conforme necessário.
Revise o desempenho anterior através dos gráficos históricos disponíveis no painel.

## Requisitos e Dependências 🛠️
Python 3.8 ou superior
Bibliotecas Python:
dash
plotly
requests
pytz
Plataforma FIWARE: Orion Context Broker e STH-Comet configurados.
Broker MQTT: Mosquitto ou equivalente para comunicação de dados.

## Licença 📄
Este projeto é de código aberto e está licenciado sob os termos da licença MIT.

## Contato 💬
Para mais informações ou dúvidas, entre em contato com os integrantes do projeto Tech Alliance:

Matheus Henriques do Amaral - 556957
Bruno Carneiro Leão - 555563
Guilherme Melo - 555310
Paulo Akira Okama - 556840
Victor Capp - 555753
