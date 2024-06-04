import os
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import linregress
import librosa
from scipy import signal
from scipy.io import wavfile


def plot_thrust_motor(file_path):
    if not os.path.isfile(file_path):
        print(f"Error: File '{file_path}' not found in the current directory.")
    else:
        # Read data from the text file
        data = []
        try:
            with open(file_path, 'r') as file:
                for line in file:
                    parts = line.strip().split(';')
                    if len(parts) == 3:
                        data.append(
                            (int(parts[0]), int(parts[1]), int(parts[2])))
        except Exception as e:
            print(f"Error reading file: {e}")
        else:
            # Separate the data into two lists: x and y
            throttle = [item[0] for item in data]
            thrust = [item[1] for item in data]
            ampere_cons = [item[2] for item in data]
            # Compute the linear regression
            slope, intercept, r_value, p_value, std_err = linregress(
                throttle[:70], thrust[:70])
            equation = f"y = {slope:.2f}x + {intercept:.2f}"
            print(equation)
            print(throttle)
            print(0.17*1350 + -176.21)
            # Plot the original data
            plt.figure(figsize=(10, 6))
            plt.scatter(throttle, thrust, color='blue', label='Original Data')

            # Plot the linear regression line
            regression_line = [slope * x + intercept for x in throttle]
            plt.plot(throttle, regression_line, color='red',
                     label=f"Linear Regression Line:\n y={slope: .2f}x + {intercept: .2f}")
            plt.legend()
            plt.grid(True)
            plt.xlabel('PWM Throttle in us')
            plt.ylabel('Thrust generated in g')
            plt.title('Thrust generated in function of the command PWM in us')
            plt.grid(True)

            # Plot the data
            plt.figure(figsize=(10, 6))
            plt.plot(throttle, [thr / ampere for thr, ampere in zip(throttle, ampere_cons)],
                     marker='o', linestyle='-', color='r')
            plt.xlabel('PWM Throttle in us')
            plt.ylabel('Motor efficiency in g/A')
            plt.title('Motor efficiency in function of the command PWM in us')
            plt.grid(True)


def plot_audio_waveform(audio_path, start_time=None, end_time=None, step_time=None):

    audio_path_mp3 = audio_path + '.mp3'
    audio_path_wav = audio_path + '.wav'

    y, sr = librosa.load(audio_path_mp3, sr=None)

    duration = librosa.get_duration(y=y, sr=sr)
    time = np.linspace(0, duration, len(y))

    # If specific start and end times are provided, select the range
    if start_time is not None and end_time is not None:
        start_sample = int(start_time * sr)
        end_sample = int(end_time * sr)
        sound_wave_amplitude = y[start_sample:end_sample]
        time = time[start_sample:end_sample]

    # Plot the detailed waveform
    plt.figure(figsize=(14, 6))
    plt.plot(time, sound_wave_amplitude, label='Amplitude')
    plt.title('Detailed Audio Waveform')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.legend(loc='upper right')

    # Add step input function at specified step_time
    step_input = np.zeros_like(time)
    step_input[time >= step_time] = 0.2
    plt.plot(time, step_input, label='Step Input', linestyle='--', color='r')
    plt.legend(loc='upper right')
    # Calculate tau, 2tau, 3tau times based on the step time
    tau_time = step_time + (1 / sr)  # tau
    two_tau_time = step_time + (2 / sr)  # 2tau
    three_tau_time = 25.5  # 3tau

    sound_wave_1150_amplitude = y[int(20 * sr):int(25 * sr)]
    sound_wave_1500_amplitude = y[int(26 * sr):int(31 * sr)]

    # Compute the FFT of the audio signal
    N_1150 = len(sound_wave_1150_amplitude)
    sound_wave_1150_magnitude = np.fft.fft(sound_wave_1150_amplitude)
    # Take the magnitude of the first half of the FFT result
    sound_wave_1150_magnitude = np.abs(sound_wave_1150_magnitude[:N_1150 // 2])
    # Compute the frequency bins
    freqs_1150 = np.fft.fftfreq(N_1150, 1/sr)[:N_1150 // 2]

    # Compute the FFT of the audio signal
    N_1500 = len(sound_wave_1500_amplitude)
    sound_wave_1500_magnitude = np.fft.fft(sound_wave_1500_amplitude)
    # Take the magnitude of the first half of the FFT result
    sound_wave_1500_magnitude = np.abs(sound_wave_1500_magnitude[:N_1500 // 2])
    # Compute the frequency bins
    freqs_1500 = np.fft.fftfreq(N_1500, 1/sr)[:N_1500 // 2]

    # Filter the FFT result to be within 0 and 1000 Hz
    idx = np.where((freqs_1500 >= 0) & (freqs_1500 <= 1000))
    freqs_1150_cutted = freqs_1150[idx]
    sound_wave_1150_magnitude_cutted = sound_wave_1150_magnitude[idx]
    freqs_1500_cutted = freqs_1500[idx]
    sound_wave_1500_magnitude_cutted = sound_wave_1500_magnitude[idx]

    # Compute the spectrogram
    # first lib
    # --------------------------------------------------------------
    D = librosa.amplitude_to_db(
        np.abs(librosa.stft(sound_wave_amplitude)), ref=np.max)

    freqs = librosa.fft_frequencies(sr=sr)
    # Filter the spectrogram to be within 0 and 1000 Hz
    idx = np.where((freqs >= 0) & (freqs <= 1000))[0]
    D = D[idx, :]
    freqs = freqs[idx]

    # Compute the time axis for the spectrogram
    times = librosa.frames_to_time(np.arange(D.shape[1]), sr=sr)

    plt.figure(figsize=(14, 6))
    librosa.display.specshow(D, sr=sr, x_axis='time', y_axis='log')
    plt.colorbar(format='%+2.0f dB')
    plt.title('Spectrogram')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Frequency (Hz)')
    plt.grid(True)
    # ----------------------------------------------------------------

    # sec lib
    # ----------------------------------------------------------------
    # Read WAV file
    sample_rate, samples = wavfile.read(audio_path_wav)
    # Define start and end times in seconds for the plot
    plot_start_time = 25.1  # in seconds
    plot_end_time = 25.4    # in seconds

    # Generate and plot the spectrogram
    plt.figure(figsize=(10, 5))
    Pxx, freqs, bins, im = plt.specgram(samples, Fs=sample_rate)

    # Set the x-axis limits to match the specified time range
    plt.xlim(plot_start_time, plot_end_time)

    # Set the y-axis limits to match the specified frequency range
    plt.ylim(0, 5000)

    # Adjust the x-axis ticks to show the correct time range
    plt.xticks(np.arange(plot_start_time, plot_end_time,
               step=(plot_end_time - plot_start_time) / 10))

    plt.xlabel('Time [s]')
    plt.ylabel('Frequency [Hz]')
    plt.title('Spectrogram')
    plt.colorbar(label='Intensity [dB]')
    plt.show()

    # frequencies, times, spectrogram = signal.spectrogram(samples, sample_rate)

    # plt.figure(figsize=(10, 5))
    # plt.imshow(spectrogram, aspect='auto', origin='lower', extent=[
    #            times.min(), times.max(), frequencies.min(), frequencies.max()])
    # plt.colorbar(label='Intensity [dB]')
    # plt.ylabel('Frequency [Hz]')
    # plt.xlabel('Time [sec]')
    # plt.title('Spectrogram')
    # plt.show()
    # ----------------------------------------------------------------

    # Plot the FFT 1150us and 1500us
    plt.figure(figsize=(14, 6))
    plt.plot(freqs_1150, sound_wave_1150_magnitude)
    plt.title('FFT of the Audio Signal 1150 magnitude')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.grid(True)

    plt.figure(figsize=(14, 6))
    plt.plot(freqs_1500, sound_wave_1500_magnitude)
    plt.title('FFT of the Audio Signal 1500 magnitude')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.grid(True)

    # Plot the FFT 1150us and 1500us
    plt.figure(figsize=(14, 6))
    plt.plot(freqs_1150_cutted, sound_wave_1150_magnitude_cutted)
    plt.title('FFT of the Audio Signal cutted 1150 magnitude')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.grid(True)

    plt.figure(figsize=(14, 6))
    plt.plot(freqs_1500_cutted, sound_wave_1500_magnitude_cutted)
    plt.title('FFT of the Audio Signal cutted 1500 magnitude')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.grid(True)

    return sound_wave_amplitude, sound_wave_1150_magnitude, sound_wave_1500_magnitude


# Check if the file exists in the current directory
file_path = 'C:/Users/Hugo/Documents/GitHub/PropellerArm/uvision_project_HDMHD/PID system modeling/MotorModelingTest/data_motor_testbench.txt'
audio_path = 'C:/Users/Hugo/Documents/GitHub/PropellerArm/uvision_project_HDMHD/PID system modeling/MotorModelingTest/sound_motor1150-1500us-stepinput'
# plot_thrust_motor(file_path)
audio_wave_amplitude = plot_audio_waveform(
    audio_path, start_time=24.7, end_time=25.7, step_time=25.1)
plt.show()
