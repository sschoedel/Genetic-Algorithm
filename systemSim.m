%second order system 
function cost = systemSim(k)
% if(isempty(k))
%     k = 0.15*ones(1,9);%controller gains 
% end
m = 1; %system mass kg
b = 15; %system damping f/v
c = 200; %system spring constant f/disp(m)
r = 200; %noise
options=odeset('RelTol',1e-7,'AbsTol',1e-7);
dx = @(t,x) [x(2);1/m*(-b*x(2)-c*x(1)-r*sin(2*x(1))+k(1)*x(2)+k(2)*x(1)+k(3)*x(2)^2+k(4)*x(1)*x(2)+k(5)*x(2)^2+k(6)*x(2)^3+k(7)*x(2)*x(1)^2+k(8)*x(1)*x(2)^2+k(9)*x(1)^3)];%
[t,x] = ode45(dx,[0,5],[0;-2],options);
timeInterval = diff(t);
dxVal = abs(x(2:length(x),1));
cost = dxVal' * timeInterval;
end