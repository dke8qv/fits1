# fits1

This exercise will begin to introduce non linear data fitting.  The examples will be based on the ROOT libraries.  Similar functionality for everything we see can be found in the numpy + scipy + lmfit + matplotlib modules. But the examples we will follow over the next few projects offer several advantages:
* far more natural histogramming tools
* completely automated fitting options ("one liners")
* convenient methods for defining custom functions and controlling the convergence of the fit
* detailed and consistent documentation
* and a low level interface to modify the objective function, running in fully optimized compiled code

You are welcome to modify the provided code for your projects and to use other packages.  Where applicable alternate examples will be included. 

* **fit1.C**: C++ function to generate random data according to a normal distribution with mean=20, sigma=10. <br> A fit is performed using the built in Gaussian model in ROOT.  Then the parameter values, their uncertainteis, and the p-value for the fit are extracted.  To run this code type ```root fit1.C``` or if you are already running ROOT, type ```.X fit1.C```  
* **fit1.py**: The same code using the python interface, run this example using ```python fit1.C```.
* For a contrast see **fit1mpl.py** for a version using matplotlib+scipy.  
* readhist.C(py):  Examples for reading the histogram files given in this example 
* ParamUnceratinties.ipynb : a guided tutorial towards most of what you will be coding in this week's exercise.
* LLexample.ipynb : a notebook giving an example for calculating (N)LLs
* TH1hist2Numpy.ipynb : an example for converting a ROOT histogram to numpy arrays

Note that from ROOT you can type ```new TBrowser()``` or in Python r.TBrowser() to get a graphical browser that allows you to look at what's contained in the TFiles.


#Exercise 1 discussion 
The 1σ width of the fitted mean distribution is about 0.3586, while the typical uncertainty reported by the fit is about 0.3257. 
These values agree well, meaning the spread of the fitted means across repeated experiments matches the uncertainty that the fit itself predicts. 
This is exactly what we expect statistically, so the fit uncertainties appear reasonable and correctly estimated.

#Exercise 2 discussion
Even though the χ² and NLL fitted mean values are both centered near the correct value of 50, the spread of the fitted means reveals the difference. 
For 10 events per fit, the expected statistical variation is σ(μ) ≈ 3.16. 
The NLL fits produce a spread consistent with this expectation (≈ 3.13), while the χ² fits give a significantly larger spread (≈ 4.27), indicating that the χ² method becomes unreliable at low statistics.


#Exercise 3 discussion
NLL(data) = 38.64
p-value = 0.44
This is exactly what we expect when the model describes the data well — a p-value somewhere around 0.3–0.7 is ideal.


#result4.pdf discussion
I scanned the likelihood and chi-square values as a function of the mean parameter,
while re-optimizing the remaining fit parameters at each point. For the likelihood fit, plotted Δ(-2 ln L), and for the chi-square fit, we plotted Δχ². 

For a single-parameter fit, the 1σ statistical uncertainty corresponds to:
Δ(-2 ln L) = 1   (likelihood method)
Δχ² = 1          (chi-square method)

From the NLL scan of histo25.root, the best-fit mean was found to be:
    μ = 53.42 ± 2.09

From the χ² scan of histo1k.root, the best-fit mean was found to be:
    μ = 49.78 ± 0.33

These uncertainties are consistent with the standard interpretation that the parameter
range where Δ(-2 ln L) = 1 or Δχ² = 1 represents the 68% confidence interval (1σ).
Thus, the fitter-reported uncertainties agree with the uncertainties extracted from
the manual likelihood and chi-square scans.
