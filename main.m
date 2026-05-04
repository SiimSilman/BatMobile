%% Batmobile Live Data Analyzer & Logger - Folder Edition
% Läser: [rawL, rawR, filtL, filtR, ctrlS, ctrlM, posS, pwmM, loopT]

% --- KONFIGURATION ---
port = 'COM4'; 
baudrate = 9600;
scrollSeconds = 15; 
folderName = 'Batmobile_Tests'; 

if ~exist(folderName, 'dir')
    mkdir(folderName);
end

baseFileName = ['test_run_', datestr(now, 'yyyy-mm-dd_HHMMSS')];
fullPathExcel = fullfile(folderName, [baseFileName, '.xlsx']);
fullPathMat   = fullfile(folderName, [baseFileName, '.mat']);

if exist('device', 'var')
    clear device
end

% --- INITIALISERA LOGG ---
dataLog = []; 
headers = {'Time_s', 'RawL_cm', 'RawR_cm', 'FiltL_cm', 'FiltR_cm', ...
           'Ctrl_Servo', 'Ctrl_Motor', 'Pos_Servo', 'PWM_Motor', 'LoopTime_ms'};

% --- INSTÄLLNINGAR DEVICE ---
device = serialport(port, baudrate);
configureTerminator(device, "LF");
flush(device);

% --- FÖRBERED FIGUR ---
fig = figure('Name', 'Batmobile Real-Time Analysis', 'Color', [0.95 0.95 0.95]);

% Stoppknapp
stopBtn = uicontrol('Style', 'pushbutton', 'String', 'STOPPA & SPARA', ...
    'Position', [20 20 120 40], 'Callback', 'setappdata(gcbf, ''run'', false)');
setappdata(fig, 'run', true);

% --- SUBPLOTS (Endast Raw och Filtered är aktiva) ---
% Vi använder subplot(2,1,x) för att de ska täcka hela bredden

subplot(2,1,1); 
rawL = animatedline('Color', 'r'); rawR = animatedline('Color', 'b');
title('1. Raw Distance (cm)'); grid on; legend('Left','Right');

subplot(2,1,2); 
filtL = animatedline('Color', 'r', 'LineWidth', 2); filtR = animatedline('Color', 'b', 'LineWidth', 2);
title('2. Filtered Distance (EMA)'); grid on; legend('Left','Right');

% --- UTKOMMENTERADE PLOTS (Sparade för framtiden) ---
% subplot(3,2,2); ctrlServo = animatedline('Color', 'm'); ctrlMotor = animatedline('Color', 'k');
% title('3. Controller Values'); grid on;
% subplot(3,2,4); yyaxis left; actServo = animatedline('Color', [0 0.5 0]); yyaxis right; actMotor = animatedline('Color', [0.8 0.4 0]);
% title('4. Actuator Output'); grid on;
% subplot(3,2,5:6); loopTimeLine = animatedline('Color', [0.2 0.7 0.2]);
% title('5. Loop Execution Time (ms)'); grid on;

startTime = datetime('now');

% --- HUVUDLOOP ---
fprintf('Loggar data till mappen: %s\n', folderName);
while getappdata(fig, 'run')
    try
        data = readline(device);
        values = str2double(split(data, ','));
        
        if numel(values) == 9
            t = seconds(datetime('now') - startTime);
            dataLog = [dataLog; t, values(:)'];
            
            % Uppdatera aktiva grafer
            addpoints(rawL, t, values(1)); 
            addpoints(rawR, t, values(2));
            addpoints(filtL, t, values(3)); 
            addpoints(filtR, t, values(4));
            
            % --- UTKOMMENTERADE DATA-PUNKTER ---
            % addpoints(ctrlServo, t, values(5)); addpoints(ctrlMotor, t, values(6));
            % addpoints(actServo, t, values(7)); addpoints(actMotor, t, values(8));
            % addpoints(loopTimeLine, t, values(9));
            
            % Uppdatera X-axlar för rullande fönster
            xlims = [t - scrollSeconds, t];
            subplot(2,1,1); xlim(xlims);
            subplot(2,1,2); xlim(xlims);
            
            % --- UTKOMMENTERADE XLIM-LOOP ---
            % for i = 1:5
            %     if i == 5, subplot(3,2,5:6); else, subplot(3,2,i); end
            %     xlim(xlims);
            % end
            
            drawnow limitrate
        end
    catch
        break;
    end
end

% --- EFTERARBETE: SPARA DATA ---
if ~isempty(dataLog)
    fprintf('\nSparar filer i %s...\n', folderName);
    
    T = array2table(dataLog, 'VariableNames', headers);
    writetable(T, fullPathExcel);
    save(fullPathMat, 'dataLog', 'headers');
    
    fprintf('Klart!\nExcel: %s\nMAT: %s\n', fullPathExcel, fullPathMat);
else
    disp('Ingen data loggades.');
end
clear device;