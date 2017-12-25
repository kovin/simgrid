%x = 2 * pi * rand (1, 200);
%y = sin (x) + sin (2 * x) + 0.2 * randn (size (x));
%## Uniform breaks
%breaks = linspace (0, 2 * pi, 41); % 41 breaks, 40 pieces
%pp1 = splinefit (x, y, breaks);
%## Breaks interpolated from data
%pp2 = splinefit (x, y, 10);  % 11 breaks, 10 pieces
%## Plot
%xx = linspace (0, 2 * pi, 400);
%y1 = ppval (pp1, xx);
%y2 = ppval (pp2, xx);
%plot (x, y, ".", xx, [y1; y2])
%axis tight
%ylim auto
%legend ({"data", "41 breaks, 40 pieces", "11 breaks, 10 pieces"})

%x = linspace(0,4*pi,10);
%y = sin(x);
%p = polyfit(x,y,7);
%x1 = linspace(0,4*pi);
%y1 = polyval(p,x1);
%figure
%plot(x,y,'o')
%hold on
%plot(x1,y1)
year = (1750:25:2000)';
pop = 1e6*[791 856 978 1050 1262 1544 1650 2532 6122 8170 11560]';
T = table(year, pop)
plot(year,pop,'o')