% simulation
END = 15;    % []
Ts = 0.08;    % []
Tcalibration = 5;
Techelon = 7;
teta_init = -pi/3;

% input_theta_truth
theta_max = 0.785398; % theta_max = 45°
freq = 0.25; % theta does not go from theta_min -> theta_max -> theta_min in less than 4s


% système
g = 9.81;    % [m/s²], gravité
d = 0.363;    % [m], distance entre l'origine et l'helice
I = 0.07110579;     % [m²kg], inertie du système
Fr = 0.1;     % []
coeff_a = 0.49;
coeff_b = 1.24;

% accéléromètre
sigma_acc = 0.022;  % [m/s²], deviation de l'accéléromètre

% gyromètre
sigma_gyr = 0.017;  % [rad/s], deviation du gyrometre
biais_gyr = 1;  % [rad/s], offset du gyromètre

% filtrage
Kteta = 20;
Ki = Kteta^2/4 - 1; % must be strictly inferior

% correcteur
K1 = 2;     % gain du correcteur proportionnel
K2 = 4 * K1; % idem mais l'autre :-)

Fsat = 1;

Fmoy = Fsat/2;
