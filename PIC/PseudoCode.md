# E(4)SC
## Pseudo código

O código a efectuar pela PIC poderá ser tratado por secções, sendo que algumas destas serão totalmente independentes entre si enquanto que outras estarão a agir diretamente no controlo de outras.

### Funcionamento

O motor necessita de 6 fases distintas para efetuar 1 rotação, sendo que em cada uma destas fases, um dos terminais do motor será levado a VCC e outro será levado a GND (HIGH Side e LOW Side, respetivamente).

O ataque de cada um destes terminais será efetuado com um mesmo sinal PWM, a uma frequência constante, e duty cycle variável, consoante a carga imposta no motor.

O arranque da fase seguinte é controlado pela passagem da fase que não foi alimentada por um ponto neutro.

Para além do funcionamento básico para otimizar a rotação do motor, o código também terá um papel importante em assegurar que o driver funciona de forma segura, através da identificação de sobre corrente.

Desta descrição do funcionamento do ESC, é possível identificar alguns blocos.

#### Frequência do PWM

Os sinais a aplicar em cada um dos terminais do motor terão que estar sincronizados, sendo que a maneira mais fácil de o efectuar será dar reset ao timer que controla a frequência do pwm quando se dá o trigger para a fase seguinte.

Como a frequência do PWM é constante, não será necessário código extenso neste bloco, sendo que apenas será necessário atuar sobre:

- A configuração do timer
- Ativação das saídas correspondentes à fase atual do sistema
- Ativação do timer de controlo do Duty Cycle
- Reset do timer na interrupção do trigger do BEMF

```C
main () {
  (...)

  T<PWM>CONbits.TCKPS = <prescaler>;
  PR<PWM> = <valor para obter frequência desejada>;
  TMR<PWM> = 0;      // Reset do timer do PWM
  T<PWM>CONbits.TON = 1; // Enable timer PWM

  (...)
}

ISR_T<PWM> () {
  TMR<PWM> = 0;                     // Reset do timer do PWM
  LATx |= (MascaraHighDaFaseAtual)
  LATx |= (MascaraLowDaFaseAtual);
  TMR<DutyCycle> = 0;               // Reset do timer do DutyCycle
  T<DutyCycle>CONbits.TON = 1;      // Enable timer DutyCycle
}

ISR_ExtInt<FaseLivre> {
  (...)
  TMR<PWM> = 0;                     // Reset do timer do PWM
  TMR<DutyCycle> = 0;               // Reset do timer do DutyCycle
  (...)
}

```

#### Controlo da Velocidade

O utilizador passará um dado set point da velocidade desejada à PIC. Com esta é possível calcular o tempo estimado em que o zero crossing devia ocorrer, e perceber se o motor está a rodar mais depressa ou mais devagar que o suposto.

Para isto é necessário um timer adicional.

**Exemplo:**
```
Velocidade Desejada = V (rpm)
Intervalo entre Zero Crossings = V / (60*6) (s)
Valor do Timer de Controlo aquando Zero Crossing = Intervalo entre Zero Crossings * Frequência do Timer de controlo - Avanço
```

Com este valor, na ISR do zero crossing, podemos verificar o valor do timer e:

- T3 < Valor do Timer de Controlo aquando Zero Crossing: velocidade demasiado elevada
- T3 > Valor do Timer de Controlo aquando Zero Crossing: velocidade demasiado baixo

Além disto, o T3 pode desempenhar a função de mudar a fase caso não seja detetada o zero crossing da fase livre.

Logo as tarefas a desempenhar são:

- Cálculo dos tempos esperados
- Na ISR do zero crossing: análise da velocidade, reset do T<ControloVelocidade>
- Na ISR do T<ControloVelocidade>: Mudar de fase, mudar o trigger da interrupção externa da fase livre, reset do timer, aumentar Dutycycle

```C
main () {
  (...)

  VelocidadeP = <UserInput>;        // Rotações por segundo
  ExpectedTimerValue = (<UserInput>/6) * PBCLK/prescaler;
  T<ControloVelocidade>CONbits.TCKPS = <prescaler>;
  PR<ControloVelocidade> =  ExpectedTimerValue;
  TMR<ControloVelocidade> = 0;      // Reset do timer do ControloVelocidade
  T<ControloVelocidade>CONbits.TON = 1; // Enable timer ControloVelocidade

  (...)

}

ISR_ExtInt<FaseLivre> {
  (...)
  delta = ExpectedTimerValue - PR<ControloVelocidade> - Avanço;
  if(delta > 0)                 //Motor demasiado rápido
        diminuirDutyCycle
  else                          //Motor demasiado lento
        aumentarDutycycle
  TMR<ControloVelocidade> = 0;  // Reset do timer do Controlo de velocidade
  (...)
}

ISR_T<ControloVelocidade> () {  // Se se entrar nesta rotina, quer dizer que o motor está demasiado lento
  CurrentFase = NextFase;
  INTCONbits.INT0EP ^= INTCONbits.INT0EP;
  aumentarDutycycle
  TMR<PWM> = 0;                     // Reset do timer do PWM
  TMR<DutyCycle> = 0;               // Reset do timer do DutyCycle
  TMR<ControloVelocidade> = 0;  // Reset do timer do Controlo de velocidade
}
```
** Nota: **
Neste exemplo, está se a considerar que a velocidade é apenas definida uma vez. Contudo, o ciclo de verificação da velocidade pedida e respetivos calculos terão de ser efetuados várias vezes ao longo do funcionamente do programa. Idealmente, a fase seria passada pela UART da PIC, permitindo assim usar as interrupções internas do periférico.

#### Duty Cycle

O duty cycle será função da carga aplicada ao motor, como mostrado acima.

Os cuidados a ter com o PWM serão:

- Desativar o timer (ou as interrupções) após entrada na ISR respetiva para evitar ter 2 ciclos HIGH no mesmo período do PWM
- Desativar as saídas da fase correspondentes

```C
ISR_T<DutyCycle> () {
  TMR<DutyCycle> = 0;                     // Reset do timer do PWM
  LATx &= !(MascaraHighDaFaseAtual)
  LATx &= !(MascaraLowDaFaseAtual);
  TMR<DutyCycle> = 0;               // Reset do timer do DutyCycle
  T<DutyCycle>CONbits.TON = 0;      // Disable timer DutyCycle
}
```

#### Deteção do zero Crossing

Apenas o trigger do terminal que não está ligado a VCC ou a GND é que pode ser considerado durante o funcionamento do circuito. Deste modo, consoante a fase em que o algoritmo se encontra, as interrupções externas correspondestes às fases que vão ser alimentadas tem que ser desligadas. Este processo tem de ser feito sempre que se atualiza a fase atual do programa.

O processo nesta fase está explicito na secção **_Controlo de Velocidade_**

```c
ISR_ExtInt<FaseFlutuante> () {
  (...)
}
```

#### Controlo de Corrente e Consumo

O controlo da corrente terá 2 vertentes:
- Monitorização de sobre-corrente
- Consumo do Controlador

O primeiro está ligado à segurança do circuito, sendo que o procedimento é controlado por uma interrupção externa. Se a rotina desta for executada, significa que o circuito corre o risco de entrar em falência, logo tem que ser tomadas medidas para desligar o controlador. Estas são:

- Disable dos timers que controlam as saídas HIN e LIN dos drivers
- Ligar shutdowns dos drivers
- Notificar utilizador
- Disable das interrupções externas

O controlo do consumo apenas está dependente da leitura da queda da resistência pela ADC e valor da tensão da bateria, em instantes definidos.

Esta leitura por parte da ADC pode ser feita com uma frequência muito mais baixa em relação à frequência de funcionamento do circuito. Para poder obter uma medida correta da energia gasta pelo circuito, o procedimento mais correto seria o uso de um timer que fizesse o pedido de leitura da ADC.

```c
ISR_T<MedidaADC> () {
  TMR<MedidaADC> = 0;                     // Reset do timer do PWM
  pedirConversãoADC();
}

ISR_ADC () {
  current = ADCCurrentValue;
  voltage = ADCCurrentValue;
  resetADCFlags();
}
```
#### Main

Da maneira como a programação do controlador está pensada, a maioria das rotinas estarão a trabalhar independentemente, através de interrupções vetorizadas.

Deste modo, o ciclo while(1) estará praticamente vazio. O início da main servirá para inicialização e configuração dos timers.

### Notas

- Devido a tantos blocos atualizarem a fase atual do algoritmo, seria sensato usar uma função que efetua-se a mudança, que fosse invocada nas diferentes ISR's do programa. Esta função seria responsável por atualizar as máscaras a ativar no PWM, por desligar as interrupções dos terminais que fossem ligados a uma das alimentações, ligar a que fica floating

```c
void nextFase(CurrentFase) {
  currentFase = nextFase;
  disableExternalInterruptHighSidePhase();
  disableExternalInterruptLowSidePhase();
  EnableExternalInterruptFloatingPhase();
}
```
