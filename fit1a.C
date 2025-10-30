#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom2.h"
#include "TStyle.h"
#include "TGraph.h"
#include <vector>
#include <iostream>

void fit1a(int entries = 1000, int ntrials = 1000)
{
    gStyle->SetOptFit(0);

    TRandom2 *generator = new TRandom2(0);

    TH1F *hReducedChi2 = new TH1F("hReducedChi2", "Reduced #chi^{2};#chi^{2}/ndf;Counts", 50, 0, 3);
    TH1F *hMeanDist     = new TH1F("hMeanDist",     "Fitted Mean Values;Mean;Counts", 50, 40, 60);
    TH1F *hProb         = new TH1F("hProb",         "#chi^{2} Probability;Prob;Counts", 50, 0, 1);
    TH1F *hMeanErrDist  = new TH1F("hMeanErrDist",  "Uncertainty on Mean;#sigma_{mean};Counts", 50, 0, 2);

    TGraph *prob_vs_redchi2 = new TGraph();
    prob_vs_redchi2->SetTitle("Probability vs Reduced #chi^{2};#chi^{2}/ndf;Probability");

    TH1F *randomHist = new TH1F("randomHist", "Random Histogram; x ; frequency", 100, 0, 100);

    for (int trial = 0; trial < ntrials; trial++)
    {
        randomHist->Reset();

        for (int i = 0; i < entries; i++)
            randomHist->Fill(generator->Gaus(50, 10));

        randomHist->Fit("gaus", "Q"); // Quiet fit
        TF1 *fit = randomHist->GetFunction("gaus");

        double chi2 = fit->GetChisquare();
        double ndf  = fit->GetNDF();
        double redChi2 = (ndf > 0 ? chi2 / ndf : 0);
        double mean = fit->GetParameter(1);
        double meanErr = fit->GetParError(1);
        double prob = fit->GetProb();

        hReducedChi2->Fill(redChi2);
        hMeanDist->Fill(mean);
        hProb->Fill(prob);
        hMeanErrDist->Fill(meanErr);

        prob_vs_redchi2->AddPoint(redChi2, prob);
    }

    // == Make 2x2 Panel Plot ==
    TCanvas *c = new TCanvas("c", "Results", 1200, 900);
    c->Divide(2, 2);

    c->cd(1); hReducedChi2->Draw();
    c->cd(2); hMeanDist->Draw();
    c->cd(3); hProb->Draw();
    c->cd(4); hMeanErrDist->Draw();

    c->SaveAs("result1.pdf");

    //Save scatter plot separately
    TCanvas *c2 = new TCanvas("c2", "Prob vs Reduced Chi2", 600, 500);
    prob_vs_redchi2->Draw("AP");
    c2->SaveAs("chi2_scatter.pdf");

    std::cout << "\nDone. Output written to result1.pdf and chi2_scatter.pdf\n";
}
