<html>
<head>
<title>Multiparton Interactions</title>
<link rel="stylesheet" type="text/css" href="pythia.css"/>
<link rel="shortcut icon" href="pythia32.gif"/>
</head>
<body>

<script language=javascript type=text/javascript>
function stopRKey(evt) {
var evt = (evt) ? evt : ((event) ? event : null);
var node = (evt.target) ? evt.target :((evt.srcElement) ? evt.srcElement : null);
if ((evt.keyCode == 13) && (node.type=="text"))
{return false;}
}

document.onkeypress = stopRKey;
</script>
<?php
if($_POST['saved'] == 1) {
if($_POST['filepath'] != "files/") {
echo "<font color='red'>SETTINGS SAVED TO FILE</font><br/><br/>"; }
else {
echo "<font color='red'>NO FILE SELECTED YET.. PLEASE DO SO </font><a href='SaveSettings.php'>HERE</a><br/><br/>"; }
}
?>

<form method='post' action='MultipartonInteractions.php'>
 
<h2>Multiparton Interactions</h2> 
 
The starting point for the multiparton interactions physics scenario in 
PYTHIA is provided by [<a href="Bibliography.php" target="page">Sjo87</a>]. Recent developments have 
included a more careful study of flavour and colour correlations, 
junction topologies and the relationship to beam remnants 
[<a href="Bibliography.php" target="page">Sjo04</a>], interleaving with initial-state radiation 
[<a href="Bibliography.php" target="page">Sjo05</a>], making use of transverse-momentum-ordered 
initial- and final-state showers, with the extension to fully 
interleaved evolution covered in [<a href="Bibliography.php" target="page">Cor10a</a>]. A framework to 
handle rescattering is described in [<a href="Bibliography.php" target="page">Cor09</a>]. 
 
<p/> 
A big unsolved issue is how the colour of all these subsystems is 
correlated. For sure there is a correlation coming from the colour 
singlet nature of the incoming beams, but in addition final-state 
colour rearrangements may change the picture. Indeed such extra 
effects appear necessary to describe data, e.g. on 
<i>&lt;pT&gt;(n_ch)</i>. A simple implementation of colour 
rearrangement is found as part of the 
<?php $filepath = $_GET["filepath"];
echo "<a href='BeamRemnants.php?filepath=".$filepath."' target='page'>";?>beam remnants</a> description. 
 
<h3>Main variables</h3> 
 
<h4>Matching to hard process</h4> 
 
The maximum <i>pT</i> to be allowed for multiparton interactions is 
related to the nature of the hard process itself. It involves a 
delicate balance between not double-counting and not leaving any 
gaps in the coverage. The best procedure may depend on information 
only the user has: how the events were generated and mixed (e.g. with 
Les Houches Accord external input), and how they are intended to be 
used. Therefore a few options are available, with a sensible default 
behaviour. 
<br/><br/><table><tr><td><strong>MultipartonInteractions:pTmaxMatch  </td><td>  &nbsp;&nbsp;(<code>default = <strong>0</strong></code>; <code>minimum = 0</code>; <code>maximum = 2</code>)</td></tr></table>
Way in which the maximum scale for multiparton interactions is set 
to match the scale of the hard process itself. 
<br/>
<input type="radio" name="1" value="0" checked="checked"><strong>0 </strong>: <b>(i)</b> if the final state of the hard process  (not counting subsequent resonance decays) contains only quarks  (<ei>u, d, s, c, b</ei>), gluons and photons then <ei>pT_max</ei>  is chosen to be the factorization scale for internal processes  and the <code>scale</code> value for Les Houches input;  <b>(ii)</b> if not, interactions are allowed to go all the way up  to the kinematical limit.  The reasoning is that the former kind of processes are generated by  the multiparton-interactions machinery and so would double-count hard  processes if allowed to overlap the same <ei>pT</ei> range,  while no such danger exists in the latter case.  <br/>
<input type="radio" name="1" value="1"><strong>1 </strong>: always use the factorization scale for an internal  process and the <code>scale</code> value for Les Houches input,  i.e. the lower value. This should avoid double-counting, but  may leave out some interactions that ought to have been simulated.  <br/>
<input type="radio" name="1" value="2"><strong>2 </strong>: always allow multiparton interactions up to the  kinematical limit. This will simulate all possible event topologies,  but may lead to double-counting.  <br/>
<br/><b>Note:</b> If a "second hard" process is present, the two 
are analyzed separately for the default 0 option. It is enough that 
one of them only consists of quarks, gluons and photons to restrict 
the <ei>pT</ei> range. The maximum for MPI is then set by the hard 
interaction with lowest scale. 
 
<h4>Cross-section parameters</h4> 
 
The rate of interactions is determined by 
<br/><br/><table><tr><td><strong>MultipartonInteractions:alphaSvalue </td><td></td><td> <input type="text" name="2" value="0.130" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.130</strong></code>; <code>minimum = 0.06</code>; <code>maximum = 0.25</code>)</td></tr></table>
The value of <i>alpha_strong</i> at <i>m_Z</i>. Default value is 
picked equal to the one used in CTEQ 5L. 
   
 
<p/> 
The actual value is then regulated by the running to the scale 
<i>pT^2</i>, at which it is evaluated 
<br/><br/><table><tr><td><strong>MultipartonInteractions:alphaSorder  </td><td>  &nbsp;&nbsp;(<code>default = <strong>1</strong></code>; <code>minimum = 0</code>; <code>maximum = 2</code>)</td></tr></table>
The order at which <ei>alpha_strong</ei> runs at scales away from 
<ei>m_Z</ei>. 
<br/>
<input type="radio" name="3" value="0"><strong>0 </strong>: zeroth order, i.e. <ei>alpha_strong</ei> is kept  fixed.<br/>
<input type="radio" name="3" value="1" checked="checked"><strong>1 </strong>: first order, which is the normal value.<br/>
<input type="radio" name="3" value="2"><strong>2 </strong>: second order. Since other parts of the code do  not go to second order there is no strong reason to use this option,  but there is also nothing wrong with it.<br/>
 
<p/> 
QED interactions are regulated by the <i>alpha_electromagnetic</i> 
value at the <i>pT^2</i> scale of an interaction. 
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:alphaEMorder  </td><td>  &nbsp;&nbsp;(<code>default = <strong>1</strong></code>; <code>minimum = -1</code>; <code>maximum = 1</code>)</td></tr></table>
The running of <ei>alpha_em</ei> used in hard processes. 
<br/>
<input type="radio" name="4" value="1" checked="checked"><strong>1 </strong>: first-order running, constrained to agree with  <code>StandardModel:alphaEMmZ</code> at the <ei>Z^0</ei> mass.  <br/>
<input type="radio" name="4" value="0"><strong>0 </strong>: zeroth order, i.e. <ei>alpha_em</ei> is kept  fixed at its value at vanishing momentum transfer.<br/>
<input type="radio" name="4" value="-1"><strong>-1 </strong>: zeroth order, i.e. <ei>alpha_em</ei> is kept  fixed, but at <code>StandardModel:alphaEMmZ</code>, i.e. its value  at the <ei>Z^0</ei> mass.  <br/>
 
<p/> 
Note that the choices of <i>alpha_strong</i> and <i>alpha_em</i> 
made here override the ones implemented in the normal process machinery, 
but only for the interactions generated by the 
<code>MultipartonInteractions</code> class. 
 
<p/> 
In addition there is the possibility of a global rescaling of 
cross sections (which could not easily be accommodated by a 
changed <i>alpha_strong</i>, since <i>alpha_strong</i> runs) 
<br/><br/><table><tr><td><strong>MultipartonInteractions:Kfactor </td><td></td><td> <input type="text" name="5" value="1.0" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>1.0</strong></code>; <code>minimum = 0.5</code>; <code>maximum = 4.0</code>)</td></tr></table>
Multiply all cross sections by this fix factor. 
   
 
<p/> 
The processes used to generate multiparton interactions form a subset 
of the standard library of hard processes. The input is slightly 
different from the standard hard-process machinery, however, 
since incoming flavours, the <i>alpha_strong</i> value and most 
of the kinematics are already fixed when the process is called. 
It is possible to regulate the set of processes that are included in the 
multiparton-interactions framework. 
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:processLevel  </td><td>  &nbsp;&nbsp;(<code>default = <strong>3</strong></code>; <code>minimum = 0</code>; <code>maximum = 3</code>)</td></tr></table>
Set of processes included in the machinery. 
<br/>
<input type="radio" name="6" value="0"><strong>0 </strong>: only the simplest <ei>2 &rarr; 2</ei> QCD processes  between quarks and gluons, giving no new flavours, i.e. dominated by  <ei>t</ei>-channel gluon exchange.<br/>
<input type="radio" name="6" value="1"><strong>1 </strong>: also <ei>2 &rarr; 2</ei> QCD processes giving new flavours  (including charm and bottom), i.e. proceeding through <ei>s</ei>-channel  gluon exchange.<br/>
<input type="radio" name="6" value="2"><strong>2 </strong>: also <ei>2 &rarr; 2</ei> processes involving one or two  photons in the final state, <ei>s</ei>-channel <ei>gamma</ei>  boson exchange and <ei>t</ei>-channel <ei>gamma/Z^0/W^+-</ei>  boson exchange.<br/>
<input type="radio" name="6" value="3" checked="checked"><strong>3 </strong>: also charmonium and bottomonium production, via  colour singlet and colour octet channels.<br/>
 
<h4>Cross-section regularization</h4> 
 
There are two complementary ways of regularizing the small-<i>pT</i> 
divergence, a sharp cutoff and a smooth dampening. These can be 
combined as desired, but it makes sense to coordinate with how the 
same issue is handled in <?php $filepath = $_GET["filepath"];
echo "<a href='SpacelikeShowers.php?filepath=".$filepath."' target='page'>";?>spacelike 
showers</a>. Actually, by default, the parameters defined here are 
used also for the spacelike showers, but this can be overridden. 
 
<p/> 
Regularization of the divergence of the QCD cross section for 
<i>pT &rarr; 0</i> is obtained by a factor <i>pT^4 / (pT0^2 + pT^2)^2</i>, 
and by using an <i>alpha_s(pT0^2 + pT^2)</i>. An energy dependence 
of the <i>pT0</i> choice is introduced by two further parameters, 
so that <i>pT0Ref</i> is the <i>pT0</i> value for the reference 
CM energy, <i>pT0Ref = pT0(ecmRef)</i>. 
<br/><b>Warning:</b> if a large <i>pT0</i> is picked for multiparton 
interactions, such that the integrated interaction cross section is 
below the nondiffractive inelastic one, this <i>pT0</i> will 
automatically be scaled down to cope. 
 
<p/> 
The actual <i>pT0</i> parameter used at a given CM energy scale, 
<i>ecmNow</i>, is obtained as 
<br/><i> 
     pT0 = pT0(ecmNow) = pT0Ref * (ecmNow / ecmRef)^ecmPow 
</i><br/> 
where <i>pT0Ref</i>, <i>ecmRef</i> and <i>ecmPow</i> are the 
three parameters below. 
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:pT0Ref </td><td></td><td> <input type="text" name="7" value="2.28" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>2.28</strong></code>; <code>minimum = 0.5</code>; <code>maximum = 10.0</code>)</td></tr></table>
The <i>pT0Ref</i> scale in the above formula. 
<br/><b>Note:</b> <i>pT0Ref</i> is one of the key parameters in a 
complete PYTHIA tune. Its value is intimately tied to a number of other 
choices, such as that of colour flow description, so unfortunately it is 
difficult to give an independent meaning to <i>pT0Ref</i>. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:ecmRef </td><td></td><td> <input type="text" name="8" value="7000.0" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>7000.0</strong></code>; <code>minimum = 1.</code>)</td></tr></table>
The <i>ecmRef</i> reference energy scale introduced above. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:ecmPow </td><td></td><td> <input type="text" name="9" value="0.215" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.215</strong></code>; <code>minimum = 0.0</code>; <code>maximum = 0.5</code>)</td></tr></table>
The <i>ecmPow</i> energy rescaling pace introduced above. 
   
 
<p/> 
Alternatively, or in combination, a sharp cut can be used. 
<br/><br/><table><tr><td><strong>MultipartonInteractions:pTmin </td><td></td><td> <input type="text" name="10" value="0.2" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.2</strong></code>; <code>minimum = 0.1</code>; <code>maximum = 10.0</code>)</td></tr></table>
Lower cutoff in <i>pT</i>, below which no further interactions 
are allowed. Normally <i>pT0</i> above would be used to provide 
the main regularization of the cross section for <i>pT &rarr; 0</i>, 
in which case <i>pTmin</i> is used  mainly for technical reasons. 
It is possible, however, to set <i>pT0Ref = 0</i> and use 
<i>pTmin</i> to provide a step-function regularization, or to 
combine them in intermediate approaches. Currently <i>pTmin</i> 
is taken to be energy-independent. 
   
 
<p/> 
G&ouml;sta Gustafson has proposed (private communication, unpublished) 
that the amount of screening, as encapsulated in the <i>pT0</i> 
parameter, fluctuates from one event to the next. Specifically, 
high-activity event are more likely to lead to interactions at large 
<i>pT</i> scales, but the high activity simultaneously leads to a 
larger screening of interactions at smaller <i>pT</i>. Such a scenario 
can approximately be simulated by scaling up the <i>pT0</i> by a 
factor <i>sqrt(n)</i>, where <i>n</i> is the number of interactions 
considered so far, including the current one. That is, for the first 
interaction the dampening factor is <i>pT^4 / (pT0^2 + pT^2)^2</i>, 
for the second <i>pT^4 / (2 pT0^2 + pT^2)^2</i>, for the third 
<i>pT^4 / (3 pT0^2 + pT^2)^2</i>, and so on. Optionally the scheme 
may also be applied to ISR emissions. For simplicity the same 
<i>alpha_s(pT0^2 + pT^2)</i> is used throughout. Note that, in this 
scenario the <i>pT0</i> scale must be lower than in the normal case 
to begin with, since it later is increased back up. Also note that the 
idea with this scenario is to propose an alternative to colour 
reconnection to understand the rise of <i>&lt;pT&gt;(n_ch)</i>, 
so that the amount of colour reconnection should be reduced. 
<br/><br/><table><tr><td><strong>MultipartonInteractions:enhanceScreening  </td><td>  &nbsp;&nbsp;(<code>default = <strong>0</strong></code>; <code>minimum = 0</code>; <code>maximum = 2</code>)</td></tr></table>
Choice to activate the above screening scenario, i.e. an increasing 
effective <ei>pT0</ei> for consecutive interactions. 
<br/>
<input type="radio" name="11" value="0" checked="checked"><strong>0 </strong>: No activity-dependent screening, i.e. <ei>pT0</ei>  is fixed.<br/>
<input type="radio" name="11" value="1"><strong>1 </strong>: The <ei>pT0</ei> scale is increased as a function  of the number of MPI's, as explained above. ISR is not affected,  but note that, if <code>SpaceShower:samePTasMPI</code> is on,  then <code>MultipartonInteractions:pT0Ref</code> is used also for ISR,  which may or may not be desirable.   <br/>
<input type="radio" name="11" value="2"><strong>2 </strong>: Both MPI and ISR influence and are influenced by the  screening. That is, the dampening is reduced based on the total number  of MPI and ISR steps considered so far, including the current one.  This dampening is implemented both for MPI and for ISR emissions,  for the latter provided that <code>SpaceShower:samePTasMPI</code> is on  (default).  <br/>
 
<h4>Impact-parameter dependence</h4> 
 
The choice of impact-parameter dependence is regulated by several 
parameters. The ones listed here refer to nondiffractive topologies 
only, while their equivalents for diffractive events are put in the 
<?php $filepath = $_GET["filepath"];
echo "<a href='Diffraction.php?filepath=".$filepath."' target='page'>";?>Diffraction</a> description. Note that 
there is currently no <code>bProfile = 4</code> option for diffraction. 
Other parameters are assumed to agree between diffractive and 
nondiffractive topologies. 
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:bProfile  </td><td>  &nbsp;&nbsp;(<code>default = <strong>3</strong></code>; <code>minimum = 0</code>; <code>maximum = 4</code>)</td></tr></table>
Choice of impact parameter profile for the incoming hadron beams. 
<br/>
<input type="radio" name="12" value="0"><strong>0 </strong>: no impact parameter dependence at all.<br/>
<input type="radio" name="12" value="1"><strong>1 </strong>: a simple Gaussian matter distribution;  no free parameters.<br/>
<input type="radio" name="12" value="2"><strong>2 </strong>: a double Gaussian matter distribution,  with the two free parameters <ei>coreRadius</ei> and  <ei>coreFraction</ei>.<br/>
<input type="radio" name="12" value="3" checked="checked"><strong>3 </strong>: an overlap function, i.e. the convolution of  the matter distributions of the two incoming hadrons, of the form  <ei>exp(- b^expPow)</ei>, where <ei>expPow</ei> is a free  parameter.<br/>
<input type="radio" name="12" value="4"><strong>4 </strong>: a Gaussian matter distribution with a width  that varies according to the selected <ei>x</ei> value of an interaction,  <ei>1. + a1 log (1 / x)</ei>, where <ei>a1</ei> is a free parameter.  Note that once <ei>b</ei> has been selected for the hard process,  it remains fixed for the remainder of the evolution. Also note that  the machinery for  <aloc href="ASecondHardProcess">a second hard process</aloc>  is not adapted to calculate the impact-parameter enhancement factor  for this option.  <br/>
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:coreRadius </td><td></td><td> <input type="text" name="13" value="0.4" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.4</strong></code>; <code>minimum = 0.1</code>; <code>maximum = 1.</code>)</td></tr></table>
When assuming a double Gaussian matter profile, <i>bProfile = 2</i>, 
the inner core is assumed to have a radius that is a factor 
<i>coreRadius</i> smaller than the rest. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:coreFraction </td><td></td><td> <input type="text" name="14" value="0.5" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.5</strong></code>; <code>minimum = 0.</code>; <code>maximum = 1.</code>)</td></tr></table>
When assuming a double Gaussian matter profile, <i>bProfile = 2</i>, 
the inner core is assumed to have a fraction <i>coreFraction</i> 
of the matter content of the hadron. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:expPow </td><td></td><td> <input type="text" name="15" value="1.85" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>1.85</strong></code>; <code>minimum = 0.4</code>; <code>maximum = 10.</code>)</td></tr></table>
When <i>bProfile = 3</i> it gives the power of the assumed overlap 
shape <i>exp(- b^expPow)</i>. Default corresponds to a simple 
exponential drop, which is not too dissimilar from the overlap 
obtained with the standard double Gaussian parameters. For 
<i>expPow = 2</i> we reduce to the simple Gaussian, <i>bProfile = 1</i>, 
and for <i>expPow &rarr; infinity</i> to no impact parameter dependence 
at all, <i>bProfile = 0</i>. For small <i>expPow</i> the program 
becomes slow and unstable, so the min limit must be respected. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:a1 </td><td></td><td> <input type="text" name="16" value="0.15" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.15</strong></code>; <code>minimum = 0.</code>; <code>maximum = 2.</code>)</td></tr></table>
When <i>bProfile = 4</i>, this gives the <i>a1</i> constant in the 
Gaussian width. When <i>a1 = 0.</i>, this reduces back to the single 
Gaussian case. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:bSelScale  </td><td>  &nbsp;&nbsp;(<code>default = <strong>1</strong></code>; <code>minimum = 1</code>; <code>maximum = 3</code>)</td></tr></table>
The selection of impact parameter is related to the scale of the 
hard process: the harder this scale is, the more central the collision. 
In practice this centrality saturates quickly, however, and beyond 
a scale of roughly 20 GeV very little changes. (The relevant 
quantity is that the QCD jet cross section above the scale should be 
a tiny fraction of the total cross section.) In <ei>2 &rarr; 1</ei> and 
<ei>2 &rarr; 2</ei> processes traditional scale choices work fine, but 
ambiguities arise for higher multiplicities, in particular when the 
scale is used for matching between the multiparton matrix elements 
and parton showers. Then the event scale may 
be chosen as that of a very low-<ei>pT</ei> parton, i.e. suggesting a 
peripheral collision, while the much harder other partons instead 
would favour a central collision. Therefore the default here is to 
override whatever scale value have been read in from an LHEF, say. 
Notice that the scale used here is decoupled from the maximum scale 
for MPIs (<code>MultipartonInteractions:pTmaxMatch</code>). 
<br/>
<input type="radio" name="17" value="1" checked="checked"><strong>1 </strong>:   Use the mass for a <ei>2 &rarr; 1</ei> process.  For <ei>2 &rarr; n, n > 1</ei> processes order the particles in  falling <ei>mmT = m + mT</ei> and then let the scale be  <ei>(mmT_1 + mmT_2)/2 + mmT_3/3 + mmT_4/4 + ... + mmT_n/n</ei>.  This is constructed always to be above <ei>m1</ei>, and to assign  decreasing importance to softer particles that are less likely to  be associated with the hard process.<br/>
<input type="radio" name="17" value="2"><strong>2 </strong>: Use the <code>scale</code> parameter of the event.  <br/>
<input type="radio" name="17" value="3"><strong>3 </strong>: use the same scale as chosen by the rules for  <code>MultipartonInteractions:pTmaxMatch</code>.<br/>
 
<h4>Rescattering</h4> 
 
It is possible that a parton may rescatter, i.e. undergo a further 
interaction subsequent to the first one. The machinery to model this 
kind of physics has only recently become fully operational 
[<a href="Bibliography.php" target="page">Cor09</a>], and is therefore not yet so well explored. 
 
<p/> 
The rescattering framework has ties with other parts of the program, 
notably with the <?php $filepath = $_GET["filepath"];
echo "<a href='BeamRemnants.php?filepath=".$filepath."' target='page'>";?>beam remnants</a>. 
 
<br/><br/><strong>MultipartonInteractions:allowRescatter</strong>  <input type="radio" name="18" value="on"><strong>On</strong>
<input type="radio" name="18" value="off" checked="checked"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>off</strong></code>)<br/>
Switch to allow rescattering of partons; on/off = true/false.<br/> 
<b>Note:</b> the rescattering framework has not yet been implemented 
for the <code>MultipartonInteractions:bProfile = 4</code> option, 
and can therefore not be switched on in that case. 
<b>Warning:</b> use with caution since machinery is still not 
so well tested. 
   
 
<br/><br/><strong>MultipartonInteractions:allowDoubleRescatter</strong>  <input type="radio" name="19" value="on"><strong>On</strong>
<input type="radio" name="19" value="off" checked="checked"><strong>Off</strong>
 &nbsp;&nbsp;(<code>default = <strong>off</strong></code>)<br/>
Switch to allow rescattering of partons, where both incoming partons 
have already rescattered; on/off = true/false. Is only used if 
<code>MultipartonInteractions:allowRescatter</code> is switched on.<br/> 
<b>Warning:</b> currently there is no complete implementation that 
combines it with shower evolution, so you must use 
<code>PartonLevel:ISR = off</code> and <code>PartonLevel:FSR = off</code>. 
If not, a warning will be issued and double rescattering will not be 
simulated. The rate also comes out to be much lower than for single 
rescattering, so to first approximation it can be neglected. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:rescatterMode  </td><td>  &nbsp;&nbsp;(<code>default = <strong>0</strong></code>; <code>minimum = 0</code>; <code>maximum = 4</code>)</td></tr></table>
Selection of which partons rescatter against unscattered partons 
from the incoming beams A and B, based on their rapidity value 
<ei>y</ei> in the collision rest frame. Here <ei>ySep</ei> is 
shorthand for <code>MultipartonInteractions:ySepRescatter</code> and 
<ei>deltaY</ei> for <code>MultipartonInteractions:deltaYRescatter</code>, 
defined below. The description is symmetric between the two beams, 
so only one case is described below. 
<br/>
<input type="radio" name="20" value="0" checked="checked"><strong>0 </strong>: only scattered partons with <ei>y > 0</ei>  can collide with unscattered partons from beam B.<br/>
<input type="radio" name="20" value="1"><strong>1 </strong>: only scattered partons with <ei>y > ySep</ei>  can collide with unscattered partons from beam B.<br/>
<input type="radio" name="20" value="2"><strong>2 </strong>: the probability for a scattered parton to be considered  as a potential rescatterer against unscattered partons in beam B increases  linearly from zero at <ei>y = ySep - deltaY</ei> to unity at  <ei>y = ySep + deltaY</ei>.<br/>
<input type="radio" name="20" value="3"><strong>3 </strong>: the probability for a scattered parton to be considered  as a potential rescatterer against unscattered partons in beam B increases  with <ei>y</ei> according to  <ei>(1/2) * (1 + tanh( (y - ySep) / deltaY))</ei>.<br/>
<input type="radio" name="20" value="4"><strong>4 </strong>: all partons are potential rescatterers against both  beams.<br/>
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:ySepRescatter </td><td></td><td> <input type="text" name="21" value="0." size="20"/>  &nbsp;&nbsp;(<code>default = <strong>0.</strong></code>)</td></tr></table>
used for some of the <code>MultipartonInteractions:rescatterMode</code> 
options above, as the rapidity for which a scattered parton has a 50% 
probability to be considered as a potential rescatterer. 
A <i>ySep > 0</i> generally implies that some central partons cannot 
rescatter at all, while a <i>ySep < 0</i>  instead allows central 
partons to scatter against either beam. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:deltaYRescatter </td><td></td><td> <input type="text" name="22" value="1." size="20"/>  &nbsp;&nbsp;(<code>default = <strong>1.</strong></code>; <code>minimum = 0.1</code>)</td></tr></table>
used for some of the <code>MultipartonInteractions:rescatterMode</code> 
options above, as the width of the rapidity transition region, where the 
probability rises from zero to unity that a scattered parton is considered 
as a potential rescatterer. 
   
 
 
<h3>Further variables</h3> 
 
These should normally not be touched. Their only function is for 
cross-checks. 
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:nQuarkIn  </td><td></td><td> <input type="text" name="23" value="5" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>5</strong></code>; <code>minimum = 0</code>; <code>maximum = 5</code>)</td></tr></table>
Number of allowed incoming quark flavours in the beams; a change 
to 4 would thus exclude <i>b</i> and <i>bbar</i> as incoming 
partons, etc. 
   
 
<br/><br/><table><tr><td><strong>MultipartonInteractions:nSample  </td><td></td><td> <input type="text" name="24" value="1000" size="20"/>  &nbsp;&nbsp;(<code>default = <strong>1000</strong></code>; <code>minimum = 100</code>)</td></tr></table>
The allowed <i>pT</i> range is split (unevenly) into 100 bins, 
and in each of these the interaction cross section is evaluated in 
<i>nSample</i> random phase space points. The full integral is used 
at initialization, and the differential one during the run as a 
"Sudakov form factor" for the choice of the hardest interaction. 
A larger number implies increased accuracy of the calculations. 
   
 
<h3>Technical notes</h3> 
 
Relative to the articles mentioned above, not much has happened. 
The main news is a technical one, that the phase space of the 
<i>2 &rarr; 2</i> (massless) QCD processes is now sampled in 
<i>dy_3 dy_4 dpT^2</i>, where <i>y_3</i> and <i>y_4</i> are 
the rapidities of the two produced partons. One can show that 
<br/><i> 
    (dx_1 / x_1) * (dx_2 / x_2) * d(tHat) = dy_3 * dy_4 * dpT^2 
</i><br/> 
Furthermore, since cross sections are dominated by the "Rutherford" 
one of <i>t</i>-channel gluon exchange, which is enhanced by a 
factor of 9/4 for each incoming gluon, effective structure functions 
are defined as 
<br/><i> 
    F(x, pT2) = (9/4) * xg(x, pT2) + sum_i xq_i(x, pT2) 
</i><br/> 
With this technical shift of factors 9/4 from cross sections to parton 
densities, a common upper estimate of 
<br/><i> 
    d(sigmaHat)/d(pT2) &lt; pi * alpha_strong^2 / pT^4 
</i><br/> 
is obtained. 
 
<p/> 
In fact this estimate can be reduced by a factor of 1/2 for the 
following reason: for any configuration <i>(y_3, y_4, pT2)</i> also 
one with <i>(y_4, y_3, pT2)</i> lies in the phase space. Not both 
of those can enjoy being enhanced by the <i>tHat &rarr; 0</i> 
singularity of 
<br/><i> 
    d(sigmaHat) propto 1/tHat^2. 
</i><br/> 
Or if they are, which is possible with identical partons like 
<i>q q &rarr; q q</i> and <i>g g &rarr; g g</i>, each singularity comes 
with half the strength. So, when integrating/averaging over the two 
configurations, the estimated <i>d(sigmaHat)/d(pT2)</i> drops. 
Actually, it drops even further, since the naive estimate above is 
based on 
<br/><i> 
    (4 /9) * (1 + (uHat/sHat)^2) &lt; 8/9 &lt; 1 
</i><br/> 
The 8/9 value would be approached for <i>tHat &rarr; 0</i>, which 
implies <i>sHat >> pT2</i> and thus a heavy parton-distribution 
penalty, while parton distributions are largest for 
<i>tHat = uHat = -sHat/2</i>, where the above expression 
evaluates to 5/9. A fudge factor is therefore introduced to go the 
final step, so it can easily be modified when further non-Rutherford 
processes are added, or should parton distributions change significantly. 
 
<p/> 
At initialization, it is assumed that 
<br/><i> 
    d(sigma)/d(pT2) &lt; d(sigmaHat)/d(pT2) * F(x_T, pT2) * F(x_T, pT2) 
       * (2 y_max(pT))^2 
</i><br/> 
where the first factor is the upper estimate as above, the second two 
the parton density sum evaluated at <i>y_3 = y_ 4 = 0</i> so that 
<i>x_1 = x_2 = x_T = 2 pT / E_cm</i>, where the product is expected 
to be maximal, and the final is the phase space for 
<i>-y_max &lt; y_{3,4} &lt; y_max</i>. 
The right-hand side expression is scanned logarithmically in <i>y</i>, 
and a <i>N</i> is determined such that it always is below 
<i>N/pT^4</i>. 
 
<p/> 
To describe the dampening of the cross section at <i>pT &rarr; 0</i> by 
colour screening, the actual cross section is multiplied by a 
regularization factor <i>(pT^2 / (pT^2 + pT0^2))^2</i>, and the 
<i>alpha_s</i> is evaluated at a scale <i>pT^2 + pT0^2</i>, 
where <i>pT0</i> is a free parameter of the order of 2 - 4 GeV. 
Since <i>pT0</i> can be energy-dependent,  an ansatz 
<br/><i> 
    pT0(ecm) = pT0Ref * (ecm/ecmRef)^ecmPow 
</i><br/> 
is used, where <i>ecm</i> is the current CM frame energy, 
<i>ecmRef</i> is an arbitrary reference energy where <i>pT0Ref</i> 
is defined, and <i>ecmPow</i> gives the energy rescaling pace. For 
technical reasons, also an absolute lower <i>pT</i> scale <i>pTmin</i>, 
by default 0.2 GeV, is introduced. In principle, it is possible to 
recover older scenarios with a sharp <i>pT</i> cutoff by setting 
<i>pT0 = 0</i> and letting <i>pTmin</i> be a larger number. 
 
<p/> 
The above scanning strategy is then slightly modified: instead of 
an upper estimate <i>N/pT^4</i> one of the form 
<i>N/(pT^2 + r * pT0^2)^2</i> is used. At first glance, <i>r = 1</i> 
would seem to be fixed by the form of the regularization procedure, 
but this does not take into account the nontrivial dependence on 
<i>alpha_s</i>, parton distributions and phase space. A better 
Monte Carlo efficiency is obtained for <i>r</i> somewhat below unity, 
and currently <i>r = 0.25</i> is hardcoded. 
 
In the generation a trial <i>pT2</i> is then selected according to 
<br/><i> 
    d(Prob)/d(pT2) = (1/sigma_ND) * N/(pT^2 + r * pT0^2)^2 * ("Sudakov") 
</i><br/> 
For the trial <i>pT2</i>, a <i>y_3</i> and a <i>y_4</i> are then 
selected, and incoming flavours according to the respective 
<i>F(x_i, pT2)</i>, and then the cross section is evaluated for this 
flavour combination. The ratio of trial/upper estimate gives the 
probability of survival. 
 
<p/> 
Actually, to profit from the factor 1/2 mentioned above, the cross 
section for the combination with <i>y_3</i> and <i>y_4</i> 
interchanged is also tried, which corresponds to exchanging <i>tHat</i> 
and <i>uHat</i>, and the average formed, while the final kinematics 
is given by the relative importance of the two. 
 
<p/> 
Furthermore, since large <i>y</i> values are disfavoured by dropping 
PDF's, a factor 
<br/><i> 
   WT_y = (1 - (y_3/y_max)^2) * (1 - (y_4/y_max)^2) 
</i><br/> 
is evaluated, and used as a survival probability before the more 
time-consuming PDF+ME evaluation, with surviving events given a 
compensating weight <i>1/WT_y</i>. 
 
<p/> 
An impact-parameter dependence is also allowed. Based on the hard 
<i>pT</i> scale of the first interaction, and enhancement/depletion 
factor is picked, which multiplies the rate of subsequent interactions. 
 
<p/> 
Parton densities are rescaled and modified to take into account the 
energy-momentum and flavours kicked out by already-considered 
interactions. 
 
<input type="hidden" name="saved" value="1"/>

<?php
echo "<input type='hidden' name='filepath' value='".$_GET["filepath"]."'/>"?>

<table width="100%"><tr><td align="right"><input type="submit" value="Save Settings" /></td></tr></table>
</form>

<?php

if($_POST["saved"] == 1)
{
$filepath = $_POST["filepath"];
$handle = fopen($filepath, 'a');

if($_POST["1"] != "0")
{
$data = "MultipartonInteractions:pTmaxMatch = ".$_POST["1"]."\n";
fwrite($handle,$data);
}
if($_POST["2"] != "0.130")
{
$data = "MultipartonInteractions:alphaSvalue = ".$_POST["2"]."\n";
fwrite($handle,$data);
}
if($_POST["3"] != "1")
{
$data = "MultipartonInteractions:alphaSorder = ".$_POST["3"]."\n";
fwrite($handle,$data);
}
if($_POST["4"] != "1")
{
$data = "MultipartonInteractions:alphaEMorder = ".$_POST["4"]."\n";
fwrite($handle,$data);
}
if($_POST["5"] != "1.0")
{
$data = "MultipartonInteractions:Kfactor = ".$_POST["5"]."\n";
fwrite($handle,$data);
}
if($_POST["6"] != "3")
{
$data = "MultipartonInteractions:processLevel = ".$_POST["6"]."\n";
fwrite($handle,$data);
}
if($_POST["7"] != "2.28")
{
$data = "MultipartonInteractions:pT0Ref = ".$_POST["7"]."\n";
fwrite($handle,$data);
}
if($_POST["8"] != "7000.0")
{
$data = "MultipartonInteractions:ecmRef = ".$_POST["8"]."\n";
fwrite($handle,$data);
}
if($_POST["9"] != "0.215")
{
$data = "MultipartonInteractions:ecmPow = ".$_POST["9"]."\n";
fwrite($handle,$data);
}
if($_POST["10"] != "0.2")
{
$data = "MultipartonInteractions:pTmin = ".$_POST["10"]."\n";
fwrite($handle,$data);
}
if($_POST["11"] != "0")
{
$data = "MultipartonInteractions:enhanceScreening = ".$_POST["11"]."\n";
fwrite($handle,$data);
}
if($_POST["12"] != "3")
{
$data = "MultipartonInteractions:bProfile = ".$_POST["12"]."\n";
fwrite($handle,$data);
}
if($_POST["13"] != "0.4")
{
$data = "MultipartonInteractions:coreRadius = ".$_POST["13"]."\n";
fwrite($handle,$data);
}
if($_POST["14"] != "0.5")
{
$data = "MultipartonInteractions:coreFraction = ".$_POST["14"]."\n";
fwrite($handle,$data);
}
if($_POST["15"] != "1.85")
{
$data = "MultipartonInteractions:expPow = ".$_POST["15"]."\n";
fwrite($handle,$data);
}
if($_POST["16"] != "0.15")
{
$data = "MultipartonInteractions:a1 = ".$_POST["16"]."\n";
fwrite($handle,$data);
}
if($_POST["17"] != "1")
{
$data = "MultipartonInteractions:bSelScale = ".$_POST["17"]."\n";
fwrite($handle,$data);
}
if($_POST["18"] != "off")
{
$data = "MultipartonInteractions:allowRescatter = ".$_POST["18"]."\n";
fwrite($handle,$data);
}
if($_POST["19"] != "off")
{
$data = "MultipartonInteractions:allowDoubleRescatter = ".$_POST["19"]."\n";
fwrite($handle,$data);
}
if($_POST["20"] != "0")
{
$data = "MultipartonInteractions:rescatterMode = ".$_POST["20"]."\n";
fwrite($handle,$data);
}
if($_POST["21"] != "0.")
{
$data = "MultipartonInteractions:ySepRescatter = ".$_POST["21"]."\n";
fwrite($handle,$data);
}
if($_POST["22"] != "1.")
{
$data = "MultipartonInteractions:deltaYRescatter = ".$_POST["22"]."\n";
fwrite($handle,$data);
}
if($_POST["23"] != "5")
{
$data = "MultipartonInteractions:nQuarkIn = ".$_POST["23"]."\n";
fwrite($handle,$data);
}
if($_POST["24"] != "1000")
{
$data = "MultipartonInteractions:nSample = ".$_POST["24"]."\n";
fwrite($handle,$data);
}
fclose($handle);
}

?>
</body>
</html>
 
<!-- Copyright (C) 2015 Torbjorn Sjostrand --> 
