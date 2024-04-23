% simulation
END = 15;    % []
Ts = 0.08;    % []

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

