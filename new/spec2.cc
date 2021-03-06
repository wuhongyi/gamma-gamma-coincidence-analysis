// spec2.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 9月 15 13:46:45 2020 (+0800)
// Last-Updated: 三 9月 16 19:17:57 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 38
// URL: http://wuhongyi.cn 

#include "TSpectrum2.h"

 
void spec2()
{
  TFile *f = new TFile("187Tl.root");

  TH2D *search;
  search = (TH2D*)f->Get("hg2");

  int nbinsx = search->GetNbinsX();
  int nbinsy = search->GetNbinsY();  
  
  Double_t ** source0 = new Double_t *[nbinsx];  
  for (int i=0; i<nbinsx; i++) source0[i] = new Double_t[nbinsy];
  Double_t ** source1 = new Double_t *[nbinsx];  
  for (int i=0; i<nbinsx; i++) source1[i] = new Double_t[nbinsy];

  for (int i = 0; i < nbinsx; i++)
    for (int j = 0; j < nbinsy; j++)
      {
	source0[i][j] = search->GetBinContent(i+1,j+1);
	source1[i][j] = search->GetBinContent(i+1,j+1);
      }

  TSpectrum2 *s = new TSpectrum2();
  // kBackIncreasingWindow  kBackDecreasingWindow
  // kBackSuccessiveFiltering  kBackOneStepFiltering

  Int_t numberIterations = 8;//参数应该是峰分辨的函数。预计在峰宽的两倍左右
  s->Background(source0,nbinsx,nbinsy,numberIterations,numberIterations,TSpectrum2::kBackIncreasingWindow,TSpectrum2::kBackSuccessiveFiltering);
  s->Background(source1,nbinsx,nbinsy,numberIterations,numberIterations,TSpectrum2::kBackIncreasingWindow,TSpectrum2::kBackOneStepFiltering);

  // 扣除连续本底但是不能去偶然符合 ,4,4,TSpectrum2::kBackDecreasingWindow,TSpectrum2::kBackSuccessiveFiltering
  // 8,8,TSpectrum2::kBackIncreasingWindow,TSpectrum2::kBackSuccessiveFiltering 不能去除偶然符合
  // 8,8,TSpectrum2::kBackIncreasingWindow,TSpectrum2::kBackOneStepFiltering  去除偶然符合
  
  TH2D *after0 = (TH2D *)search->Clone("hg30");
  TH2D *after1 = (TH2D *)search->Clone("hg31");
  // after->Reset();
  for (int i = 0; i < nbinsx; i++)
    for (int j = 0; j < nbinsy; j++)
      {
	after0->SetBinContent(i+1,j+1,after0->GetBinContent(i+1,j+1)-source0[i][j]);
	after1->SetBinContent(i+1,j+1,after1->GetBinContent(i+1,j+1)-source1[i][j]);
      }

  TH1D *hh = search->ProjectionX("hh",1,search->GetNbinsY());
  TH1D *h0 = after0->ProjectionX("h0",1,after0->GetNbinsY());
  TH1D *h1 = after1->ProjectionX("h1",1,after1->GetNbinsY());
  hh->SetLineColor(1);
  h0->SetLineColor(2);
  h1->SetLineColor(3);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,2);
  c1->cd(1);
  search->Draw("colz");
  c1->cd(2);
  after0->Draw("colz");
  c1->cd(3);
  after1->Draw("colz");
  c1->cd(4);
  hh->Draw();
  h0->Draw("same");
  h1->Draw("same");
  
  return;
}


// 
// spec2.cc ends here
