#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TRandom2.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TMath.h"
#include <iostream>

// --------------------------------------------------------
// Compute NLL based on Poisson likelihood per bin
// --------------------------------------------------------
double computeNLL(TH1F *h, TF1 *g)
{
    double nll = 0;
    int nb = h->GetNbinsX();

    // Normalize function to have same total area as histogram
    double totalData = h->Integral();
    double totalFit = g->Integral(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    double scale = totalData / totalFit;

    for(int i=1; i<=nb; i++){
        double n = h->GetBinContent(i);
        double x1 = h->GetXaxis()->GetBinLowEdge(i);
        double x2 = h->GetXaxis()->GetBinUpEdge(i);

        double E = scale * g->Integral(x1, x2);

        if(E > 0){
            if(n > 0)
                nll += -(n * log(E) - E);
            else
                nll += -(-E);
        }
    }
    return nll;
}

// --------------------------------------------------------
// Main
// --------------------------------------------------------
void exercise3()
{
    // Load histogram
    TFile *f = TFile::Open("histo25.root");
    TH1F *h = (TH1F*)f->Get("randomHist1");

    // Perform NLL fit
    h->Fit("gaus","LQ");
    TF1 *g = h->GetFunction("gaus");

    // Compute NLL for data
    double nll_data = computeNLL(h, g);
    std::cout << "NLL(data) = " << nll_data << std::endl;

    // Book histogram for toy NLL values
    TH1F *hNLL = new TH1F("hNLL","NLL Toy Distribution;NLL;Counts",60,nll_data-30,nll_data+30);

    int ntoy = 5000;
    TRandom2 rand(0);

    // Generate toy experiments
    for(int t=0; t<ntoy; t++){
        TH1F *hToy = (TH1F*)h->Clone("hToy");
        hToy->Reset();

        int nb = h->GetNbinsX();
        double totalData = h->Integral();
        double totalFit = g->Integral(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
        double scale = totalData / totalFit;

        for(int i=1; i<=nb; i++){
            double x1 = h->GetXaxis()->GetBinLowEdge(i);
            double x2 = h->GetXaxis()->GetBinUpEdge(i);
            double Ei = scale * g->Integral(x1, x2);
            int ni = rand.Poisson(Ei);
            hToy->SetBinContent(i, ni);
        }

        double nll_toy = computeNLL(hToy, g);
        hNLL->Fill(nll_toy);

        delete hToy;
    }

    // Draw and save result
    TCanvas *c = new TCanvas("c","NLL Distribution",800,600);
    hNLL->Draw();
    TLine *line = new TLine(nll_data,0,nll_data,hNLL->GetMaximum());
    line->SetLineColor(kRed);
    line->SetLineWidth(3);
    line->Draw("same");
    c->SaveAs("result3.pdf");

    // Compute p-value
    int count = 0;
    for(int i=1; i<=hNLL->GetNbinsX(); i++){
        if(hNLL->GetBinCenter(i) >= nll_data)
            count += hNLL->GetBinContent(i);
    }
    double pvalue = double(count)/ntoy;

    std::cout << "p-value = " << pvalue << std::endl;
}
