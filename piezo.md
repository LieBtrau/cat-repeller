# Acoustic Resonance (?)
1. Connect the piezo element directly to the output of the signal generator.
2. Set the signal generator to sine wave mode, no DC-offset.
3. Connect an oscilloscope to the signal generator output
3. Sweep the frequency range from 1kHz to 100kHz.
4. Watch the oscilloscope for a dip in the sine wave.

For the piezo elements of the Delta-Sport cat repeller, that frequency is 22kHz.

Is this the acoustic resonance?  When doing the same test with another piezo element, the frequency where the dip occurs was the loudest.

# Capacitance of the piezo elements
The capacitance of the Delta-Sport elements is around 115nF

# Parallel inductor
We need an inductor to form an parallel LC-resonant circuit at double the acoustic frequency.

$$
L = \frac{1}{(2*\pi*f)^2*C} = \frac{1}{(2*\pi*44k)^2*115n} = 114µH
$$

By trial and error, I tried several inductors to find the one that yielded the highest voltage across the piezo element.  The inductor that worked best was between 120µH and 150µH.