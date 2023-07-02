pkg load symbolic
syms psi phi theta psidot phidot thetadot

RzPsi = [[cos(psi), -sin(psi), 0]; [sin(psi), cos(psi), 0]; [0,0,1]]
RyTheta = [[cos(theta), 0, sin(theta)];[0,1,0];[-sin(theta), 0, cos(theta)]]
RzPhi = [[cos(phi), -sin(phi), 0]; [sin(phi), cos(phi), 0]; [0,0,1]]

RzPhi * RyTheta
