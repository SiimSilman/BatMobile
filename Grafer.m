clearvars; clc; close all;

%% Data för Scenario 1
Mean1 = [52.88, 57.44, 56.26, 53.43, 46.73, 58.34, 70.02, 60.86, 60.66, 67.92];
STD1 = [18.94, 14.72, 15.61, 13.60, 15.07, 21.81, 22.74, 18.63, 18.34, 17.88];
x1 = 1:length(Mean1);

% Linjäranpassning Scenario 1
p1 = polyfit(x1, Mean1, 1);
y_fit1 = polyval(p1, x1);

%% Data för Scenario 2
Mean2 = [46.22, 48.08, 51.71, 43.3, 46.78, 47.28, 52.97, 53.38, 49.18, 53.02];
STD2 = [15.31, 15.63, 16.14, 11.11, 8.12, 10.66, 23.42, 25.33, 18.65, 25.42];
x2 = 1:length(Mean2);

% Linjäranpassning Scenario 2
p2 = polyfit(x2, Mean2, 1);
y_fit2 = polyval(p2, x2);

%% Plot Scenario 1
figure('Name', 'Scenario 1 - Tracking Performance', 'Color', 'w');
hold on; grid on;

% Rita ut standardavvikelse som Error Bars (ser renare ut än stjärnor)
errorbar(x1, Mean1, STD1, 'o', 'Color', [0.4 0.4 0.4], 'MarkerSize', 6, ...
    'MarkerFaceColor', 'r', 'LineWidth', 1.2, 'DisplayName', 'Trial Mean & STD');

% Linjäranpassning
plot(x1, y_fit1, '-', 'Color', 'b', 'LineWidth', 2, 'DisplayName', 'Linear Trend');

% Referenslinje
yline(30, '--k', 'LineWidth', 1.5, 'DisplayName', 'Reference (30 cm)');

% Totalmedelvärde (I-del improvement)
yline(mean(Mean1), ':', 'Color', [0.1 0.6 0.1], 'LineWidth', 1.5, 'DisplayName', 'Grand Mean');

% Formatering
xlabel('Trial Number');
ylabel('Distance (cm)');
title('Tracking Performance: Scenario 1');
ylim([0 100]);
xlim([0.5 length(Mean1)+0.5]);
legend('Location', 'northeastoutside');
set(gca, 'FontSize', 11, 'TickLabelInterpreter', 'latex');

%% Plot Scenario 2
figure('Name', 'Scenario 2 - Tracking Performance', 'Color', 'w');
hold on; grid on;

errorbar(x2, Mean2, STD2, 'o', 'Color', [0.4 0.4 0.4], 'MarkerSize', 8, ...
    'MarkerFaceColor', 'r', 'LineWidth', 1.2, 'DisplayName', 'Trial Mean & STD');

plot(x2, y_fit2, '-', 'Color', 'b', 'LineWidth', 2, 'DisplayName', 'Linear Trend');

yline(30, '--k', 'LineWidth', 1.5, 'DisplayName', 'Reference (30 cm)');
yline(mean(Mean2), ':', 'Color', [0.1 0.6 0.1], 'LineWidth', 1.5, 'DisplayName', 'Grand Mean');

xlabel('Trial Number');
ylabel('Distance (cm)');
title('Tracking Performance: Scenario 2');
ylim([0 100]);
xlim([0.5 length(Mean2)+0.5]);
legend('Location', 'northeastoutside');
set(gca, 'FontSize', 11, 'TickLabelInterpreter', 'latex');

% Skriv ut ekvationerna i kommandofönstret
fprintf('Scenario 1: y = %.2fx + %.2f\n', p1(1), p1(2));
fprintf('Scenario 2: y = %.2fx + %.2f\n', p2(1), p2(2));