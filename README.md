[![Build status](https://ci.appveyor.com/api/projects/status/5jkfbf61yakm7ssl?svg=true)](https://ci.appveyor.com/project/marcosd4h/sysmonx)
[![Appveyor](https://badgen.net/appveyor/ci/marcosd4h/sysmonx)](https://ci.appveyor.com/project/marcosd4h/sysmonx)
[![Latest Commit](https://badgen.net/github/last-commit/marcosd4h/sysmonx)](https://github.com/marcosd4h/sysmonx/commits/master)
[![MIT license](https://badgen.net/badge/license/MIT/blue)](http://opensource.org/licenses/MIT)

<p align="center">
  <br>
  <img src="https://raw.githubusercontent.com/marcosd4h/sysmonx/master/docs/misc/SysmonX_small.jpg">
</p>

# SysmonX
Open Source drop-in compatible version of sysmon
>  This project is WORK IN PROGRESS. Expect feature changes and binary releases on the upcoming weeks.


## Overview
SysmonX is an open-source, community-driven, and drop-in replacement version of Sysmon that provides a modularized architecture with the purpose of enabling the infosec community to:

- Extend the Sysmon data collection sources and create new security events 
- Extend the Sysmon ability to correlate events. Effectively enabling new logical operations between events and the creation of  advanced detection capabilities 
- Enrich the current set of events with more data! 
- Enable the false positive reduction by narrowing down   suspicious events through dedicated scanners 
- Extend the security configuration schema 
- React to known subversion and evasion techniques that impact Sysmon, and by doing so, increasing the resilience of security auditing and data collection mechanism such as this one.

SysmonX is composed of a standalone binary that gets itself deployed as a windows service, supports legacy Sysmon configurations and event reporting mechanism, while also providing users the ability to configure all the SysmonX aspects through command-line interface. 

The SysmonX binary is a drop-in replacement of Sysmon. This effectively means that SysmonX is a feature-compatible version of Sysmon (same input, same output). This is possible thanks to the SysmonX ability to package, deploy, manage Sysmon binaries behind the scene. SysmonX uses this to intercept data collected by Sysmon drivers, enrich them, along with the ability to create, combine, and add scanning logic on top of new security events. The result is a combined output, with the old good features from Sysmon + the new features from SysmonX.

Example of new security events and features added to SysmonX are:
- Cmdline and Parent Process Spoofing detection
- WMI calls over all the namespaces, not just root:subscription
- Ability to collect authentication information
- Ability to collect powershell events
- Ability to detect userspace injection techniques (eventing + memory inspection through built in scanner modules)
- Ability to perform regex over security event fields
- Many more!

### SysmonX Overview deck available [here](https://github.com/marcosd4h/sysmonx/blob/master/docs/slides/SysmonX_Overview_09022019.pdf)

### SysmonX Overview Talk - Video [here](https://www.youtube.com/watch?v=bUgZIc7zfyk)

### SysmonX Demo: Component Install, Component Uninstall and Regex Detection - Video [here](https://www.youtube.com/watch?v=Z5B1T0P7sfs ) <br> <img src="https://thumbs.gfycat.com/IllustriousSpeedyArawana-size_restricted.gif" data-canonical-src="https://thumbs.gfycat.com/IllustriousSpeedyArawana-size_restricted.gif" "height="600" />
