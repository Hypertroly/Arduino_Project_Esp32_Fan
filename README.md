# Projeto de Objetos Inteligentes Conectados

Este é um projeto da matéria de objetos inteligentes conectados da Universidade Presbiteriana Mackenzie.
O projeto é constituído de uma placa Esp32 Módulo WiFi que gerencia um sensor de temperatura e um módulo rele para ligar ou desligar um mini ventilador
O código cria uma conexão com o celular no aplicativo MQTT Dash a partir do WiFi e um broker (eclipse) utilizando o protocolo MQTT. Pela conexão, pode-se escolher 4 modos:

1- Ventilador Ligado
2- Ventilador Desligado
3- Automático ativado
4- Automático desativado

No modo ativado, o ventilador é ligado quando a temperatura ambiente, captada pelo sensor TMP35, chega ou passa de um certo número, o qual pode ser mudado.


![image](https://user-images.githubusercontent.com/43524597/120084710-e9886180-c0a8-11eb-8ba1-d79245ff8e34.png)
![image](https://user-images.githubusercontent.com/43524597/120086046-8819c000-c0b3-11eb-8fd1-55c6d70fdc69.png)
![image](https://user-images.githubusercontent.com/43524597/120086086-a97aac00-c0b3-11eb-8853-fde79e275e0b.png)
