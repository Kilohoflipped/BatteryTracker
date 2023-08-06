classdef FFTAnalyzer < handle
    
    properties
        vData
        iData
        fs
        ts
    end
    
    methods
        
        function obj = FFTAnalyzer(vData, iData, fs)
            % 构造函数，初始化数据和采样频率
            obj.vData = vData;
            obj.iData = iData;
            obj.fs = fs;
            obj.ts = 1/fs;
            %obj.analyzeImpedance();
        end
        
        function [P1V,fv,P1I,fi] = fftAnalis(obj)
            fftV = fft(obj.vData);
            LV = length(obj.vData);
            P2V = abs(fftV/LV);
            P1V = P2V(1:LV/2+1);
            P1V(2:end-1) = 2*P1V(2:end-1);
            fv = obj.fs*(0:(LV/2))/LV;  

            fftI = fft(obj.iData);
            LI = length(obj.iData);
            P2I = abs(fftI/LI);
            P1I = P2I(1:LI/2+1);
            P1I(2:end-1) = 2*P1I(2:end-1);
            fi = obj.fs*(0:(LV/2))/LI; 
        end

        function analyzeImpedance(obj)
            % 分析阻抗谱
            
            % 确定采样间隔
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