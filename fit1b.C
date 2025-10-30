#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom2.h"
#include "TStyle.h"
#include <iostream>

void fit1b(int entries = 10, int ntrials = 1000)
{
    gStyle->SetOptFit(0);

    TRandom2 rand(0);

    TH1F *hMeanChi2 = new TH1F("hMeanChi2", "Mean from #chi^{2} fits;Mean;Counts", 60, 40, 60);
    TH1F *hMeanNLL  = new TH1F("hMeanNLL",  "Mean from NLL fits;Mean;Counts", 60, 40, 60);

    TH1F *h = new TH1F("h","",100,0,100);

    for(int t=0; t<ntrials; t++)
    {
        h->Reset();
        for(int i=0;i<entries;i++)
            h->Fill(rand.Gaus(50,10));

        // χ2 Fit
        h->Fit("gaus","Q");  
        TF1 *f1 = h->GetFunction("gaus");
        hMeanChi2->Fill(f1->GetParameter(1));

        // NLL Fit
        h->Fit("gaus","QL"); 
        TF1 *f2 = h->GetFunction("gaus");
        hMeanNLL->Fill(f2->GetParameter(1));
    }

    TCanvas *c = new TCanvas("c","Comparison",1200,600);
    c->Divide(2,1);

    c->cd(1); hMeanChi2->Draw();
    c->cd(2); hMeanNLL->Draw();

    c->SaveAs("result2.pdf");

    std::cout << "Saved result2.pdf" << std::endl;
}
