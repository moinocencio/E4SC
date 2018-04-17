%% Analize Differential Amplifier

values = dlmread('VasVs.txt','',2,0);
time = values(:,1);
Vs = values(:,2);
Vad = values(:,3);

%%
close all

Gain = 10;
tVad = Vs*Gain;
figure(1)
plot(time,Vs,time,Vad-2.5,time,tVad+1)
legend('Shunt','Amplified Voltage','Theoretical Amplified Voltage')
title('Voltages with offsets applied')