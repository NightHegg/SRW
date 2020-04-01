clc
format shortG
format loose
y=importdata("results/y.dat");
Sigma=importdata("results/Sigma.dat");
Imp_Data=importdata("files/mainData.dat");
Add_Data=importdata("files/addData.dat");
N=Add_Data(1);
a=Imp_Data(1,1);
b=Imp_Data(1,2);
pa=Imp_Data(2,1);
pb=Imp_Data(2,2);
E=Imp_Data(3,1);
nyu=Imp_Data(3,2);
uk=Imp_Data(4,1);
rk=Imp_Data(4,2);

y_an=@(r)(((1-2*nyu)*(  1+nyu)/E)*((pa*a^2-pb*b^2)/(b^2-a^2))*r+((1+nyu)/E)*((a^2+b^2)/r)*...
    ((pa-pb)/(b^2-a^2)))*uk;
SigmaR_an=@(r)(((pa.*a.^2-pb.*b.^2)/(b.^2-a.^2))-(((a.^2*b.^2)./(r.^2))*((pa-pb)./(b.^2-a.^2)))).*rk;
SigmaT_an=@(r)(((pa.*a.^2-pb.*b.^2)/(b.^2-a.^2))+(((a.^2*b.^2)./(r.^2))*((pa-pb)./(b.^2-a.^2)))).*rk;
r=linspace(a,b,10);

SR1_1=fplot(SigmaR_an,[a b],"r-");
set( SR1_1, 'LineWidth', 2 );
hold on
SR1_2=plot(r,Sigma(1:N/10:end,1),"bo");
set( SR1_2, 'LineWidth', 2 );
axis auto
l1=legend("Analytical","Numerical","Location","southeast");
%saveas(SR1_2,"SRW_Text/graphs/SigmaR.png");
hold on
SR2_1=fplot(SigmaT_an,[a b],"r-");
set( SR2_1, 'LineWidth', 2 );
hold on
SR2_2=scatter(r,Sigma(1:N/10:end,2));
set( SR2_2, 'LineWidth', 2 );
axis auto
l2=legend("Analytical","Numerical","Location","northeast");
%saveas(SR2_2,"SRW_Text/graphs/SigmaT.png");

