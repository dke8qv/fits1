#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TMath.h"
#include <iostream>

// ---------- Compute NLL ----------
double computeNLL(TH1F *h, TF1 *g)
{
    double nll = 0;
    int nb = h->GetNbinsX();

    double totalData = h->Integral();
    double totalFit = g->Integral(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    double scale = totalData / totalFit;

    for(int i=1; i<=nb; i++){
        double n = h->GetBinContent(i);
        double x1 = h->GetXaxis()->GetBinLowEdge(i);
        double x2 = h->GetXaxis()->GetBinUpEdge(i);
        double E = scale * g->Integral(x1,x2);

        if(E > 0) {
            if(n > 0) nll += -(n*log(E) - E);
            else      nll += -(-E);
        }
    }
    return nll;
}


// ---------- MAIN ----------
void result4()
{
    // ====== NLL SCAN (histo25.root) ======
    TFile *f1 = TFile::Open("histo25.root");
    TH1F *h1 = (TH1F*)f1->Get("randomHist1");

    TF1 *g1 = new TF1("g1","gaus",h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
    g1->SetParameters(h1->GetMaximum(), h1->GetMean(), h1->GetRMS());
    h1->Fit(g1,"LQ");

    double mean0 = g1->GetParameter(1);
    double mean_err = g1->GetParError(1);
    double NLL_min = computeNLL(h1,g1);

    TGraph *grNLL = new TGraph();
    for(double mu = mean0 - 4*mean_err; mu <= mean0 + 4*mean_err; mu+=0.02){
        g1->FixParameter(1, mu);
        h1->Fit(g1,"LQ0");
        g1->ReleaseParameter(1);

        double nll = computeNLL(h1,g1);
        grNLL->SetPoint(grNLL->GetN(), mu, 2*(nll - NLL_min));
    }


    // ====== Chi2 SCAN (histo1k.root) ======
    TFile *f2 = TFile::Open("histo1k.root");
    TH1F *h2 = (TH1F*)f2->Get("randomHist1");

    TF1 *g2 = new TF1("g2","gaus",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
    g2->SetParameters(h2->GetMaximum(), h2->GetMean(), h2->GetRMS());
    h2->Fit(g2,"Q");

    double mean02 = g2->GetParameter(1);
    double mean_err2 = g2->GetParError(1);
    double chi2_min = g2->GetChisquare();

    TGraph *grChi2 = new TGraph();
    for(double mu = mean02 - 4*mean_err2; mu <= mean02 + 4*mean_err2; mu+=0.02){
        g2->FixParameter(1, mu);
        h2->Fit(g2,"Q0");
        g2->ReleaseParameter(1);

        double chi2 = g2->GetChisquare();
        grChi2->SetPoint(grChi2->GetN(), mu, chi2 - chi2_min);
    }


    // ====== DRAW BOTH TO ONE PDF ======
    TCanvas *c = new TCanvas("c","result4",1200,500);
    c->Divide(2,1);

    c->cd(1);
    grNLL->SetTitle("NLL Scan;Mean;#Delta(-2 ln L)");
    grNLL->Draw("AL");
    TLine *l1 = new TLine(mean0,0,mean0,4);
    l1->SetLineColor(kRed); l1->SetLineWidth(2); l1->Draw("same");
    TLine *l2 = new TLine(mean0-mean_err,1,mean0+mean_err,1);
    l2->SetLineColor(kBlue); l2->SetLineWidth(2); l2->Draw("same");

    c->cd(2);
    grChi2->SetTitle("Chi-Square Scan;Mean;#Delta#chi^{2}");
    grChi2->Draw("AL");
    TLine *l3 = new TLine(mean02,0,mean02,4);
    l3->SetLineColor(kRed); l3->SetLineWidth(2); l3->Draw("same");
    TLine *l4 = new TLine(mean02-mean_err2,1,mean02+mean_err2,1);
    l4->SetLineColor(kBlue); l4->SetLineWidth(2); l4->Draw("same");

    c->SaveAs("result4.pdf");


    // Print summary
    std::cout << "\n===== Results =====" << std::endl;
    std::cout << "NLL Fit (histo25): mean = " << mean0 << " ± " << mean_err << std::endl;
    std::cout << "Chi2 Fit (histo1k): mean = " << mean02 << " ± " << mean_err2 << std::endl;
    std::cout << "Plots saved to result4.pdf" << std::endl;
}

