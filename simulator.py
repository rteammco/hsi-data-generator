import numpy as np
import matplotlib.pyplot as plt

def read_spectra():
  spectra_file = open('spectra.txt', 'r')
  all_spectra = []
  element = ''
  spectrum = []
  on_spectrum = False
  for line in spectra_file:
    line = line.strip()
    if not on_spectrum:
      on_spectrum = True
      element = line
    elif line == '':
      all_spectra.append((element, spectrum))
      on_spectrum = False
      spectrum = []
    else:
      spectrum += map(float, line.split())
  if len(spectrum) > 0:
    all_spectra.append((element, spectrum))
  return all_spectra

def plot_data():
  spectra = read_spectra()
  for title_and_spectrum in spectra:
    title = title_and_spectrum[0]
    spectrum = title_and_spectrum[1]
    plt.plot(range(len(spectrum)), spectrum)
    plt.title(title)
    plt.show()

if __name__ == '__main__':
  plot_data()
