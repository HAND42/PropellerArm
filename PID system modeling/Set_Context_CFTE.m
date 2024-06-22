% simulation
END = 15;    % []
Ts = 0.08;    % []
Tcalibration = 0.5;
Techelon = 4    ;

% input_theta_truth
theta_max = 0.785398; % theta_max = 45°
freq = 0.5; % theta does not go from theta_min -> theta_max -> theta_min in less than 2s

% système
g = 9.81;    % [m/s²], gravité

% accéléromètre
sigma_acc = 0.022;  % [m/s²], deviation de l'accéléromètre

% gyromètre
sigma_gyr = 0.01047;  % [rad/s], deviation du gyrometre
biais_gyr = 1;  % [rad/s], offset du gyromètre

% filtrage
Kteta = 20;
Ki = Kteta^2/4 - 1; % must be strictly inferior

theta_ref = -pi()/4;
K1 = 4;
K2 = 128;

fr = 0.1;
l2 = 0.363;
l1 = l2;

I = 0.07110579;

PWM_moy = 1350; %us
PWM_max = 1750; %us
PWM_min = 1050; %us
Fmoy = 0.17*PWM_moy + -176.21; % 53.29 N
Fmax = 0.17*PWM_max + -176.21; % 121.29 N
Fmin = 0.17*PWM_min + -176.21; % 2.29 N

Throttle = [1050, 1060, 1070, 1080, 1090, 1100, 1110, 1120, 1130, 1140, 1150, 1160, 1170, 1180, 1190, 1200, 1210, 1220, 1230, 1240, 1250, 1260, 1270, 1280, 1290, 1300, 1310, 1320, 1330, 1340, 1350, 1360, 1370, 1380, 1390, 1400, 1410, 1420, 1430, 1440, 1450, 1460, 1470, 1480, 1490, 1500, 1510, 1520, 1530, 1540, 1550, 1560, 1570, 1580, 1590, 1600, 1610, 1620, 1630, 1640, 1650, 1660, 1670, 1680, 1690, 1700, 1710, 1720, 1730, 1740, 1750, 1760, 1770, 1780, 1790, 1800, 1810, 1820, 1830, 1840, 1850, 1860, 1870, 1880, 1890, 1900];
MotorFunction = 0.17*Throttle + -176.21;


