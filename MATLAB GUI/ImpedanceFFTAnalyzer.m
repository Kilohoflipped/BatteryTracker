classdef ImpedanceFFTAnalyzer < handle
    
    properties
        xData
        yData
        fs
    end
    
    methods
        
        function obj = ImpedanceFFTAnalyzer(~, ~, ~)
            % 构造函数，初始化数据和采样频率
            obj.yData = [1	2	3	4	1	2	3	4	1	2	3	4];
            obj.xData = [1	2	3	4	5	6	7	8	9	10	11	12];
            obj.fs = 5;
            obj.analyzeImpedance();
        end
        
        function analyzeImpedance(obj)
            % 分析阻抗谱
            
            % 确定采样频率
            x = obj.xData - 39.5;
            y = obj.yData - 376.38;
            dt = 1 / obj.fs;

            % 计算FFT
            N_x = length(x); % 原始信号长度
            X = fft(x); % 计算FFT

            % 计算频率轴
            fx = (0:N_x/2) * obj.fs / N_x;

            % 计算单边幅度谱，排除直流分量的影响
            amplitude_x = abs(X(1:N_x/2+1)) * 2 / N_x; % 计算单边幅度谱
            dc_amp_x = abs(X(1)) / N_x; % 直流分量的幅度
            amplitude_x(1) = dc_amp_x; % 将直流分量的幅度赋值给单边幅度谱的第一个点

            % 绘制单边幅度谱
            figure(1), plot(fx, amplitude_x);
            xlim([0 1e3])
            xlabel('Frequency (Hz)');
            ylabel('Amplitude');
            title('Single-Sided Amplitude Spectrum');

            % 计算FFT
            N_y = length(y); % 原始信号长度
            Y = fft(y); % 计算FFT

            % 计算频率轴
            fy = (0:N_y/2) * obj.fs / N_y;

            % 计算单边幅度谱，排除直流分量的影响
            amplitude_y = abs(Y(1:N_y/2+1)) * 2 / N_y; % 计算单边幅度谱
            dc_amp_y = abs(X(1)) / N_y; % 直流分量的幅度
            amplitude_y(1) = dc_amp_y; % 将直流分量的幅度赋值给单边幅度谱的第一个点

            % 绘制单边幅度谱
            figure(2), plot(fy, amplitude_y);
            xlim([0 10e3])
            xlabel('Frequency (Hz)');
            ylabel('Amplitude');
            title('Single-Sided Amplitude Spectrum');

            Z_fft = zeros(1, 12); % 初始化幅值数组
            for i = 1:12
                Z_fft(i) = Y(i) / X(i);
            end

            % 绘制阻抗谱
            figure;
            plot(real(Z_fft),abs(imag(Z_fft)));
            xlabel('Real(Z)');
            ylabel('Imag(Z)');
            title('Impedance Spectrum');
            grid on;
        end
    end
end