<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="HHK-Bluetooth">
<packages>
<package name="DIL28-3">
<description>&lt;B&gt;Dual In Line&lt;/B&gt;&lt;p&gt;
package type P</description>
<wire x1="-17.78" y1="-1.27" x2="-17.78" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="1.27" x2="-17.78" y2="-1.27" width="0.1524" layer="21" curve="-180"/>
<wire x1="17.78" y1="-2.54" x2="17.78" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="2.54" x2="-17.78" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="2.54" x2="17.78" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-17.653" y1="-2.54" x2="17.78" y2="-2.54" width="0.1524" layer="21"/>
<pad name="1" x="-16.51" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-13.97" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-11.43" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-8.89" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-6.35" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-3.81" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="-1.27" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="1.27" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="3.81" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="6.35" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="8.89" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="11.43" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="13.97" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="16.51" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="16.51" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="13.97" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="11.43" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="8.89" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="6.35" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="3.81" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="1.27" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="-1.27" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="-3.81" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="-6.35" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="25" x="-8.89" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="26" x="-11.43" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="27" x="-13.97" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="28" x="-16.51" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<text x="-17.907" y="-2.54" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-15.748" y="-0.9398" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="PIC24FJ64GB002">
<wire x1="-25.4" y1="-20.32" x2="25.4" y2="-20.32" width="0.254" layer="94"/>
<wire x1="25.4" y1="-20.32" x2="25.4" y2="17.78" width="0.254" layer="94"/>
<wire x1="25.4" y1="17.78" x2="-25.4" y2="17.78" width="0.254" layer="94"/>
<wire x1="-25.4" y1="17.78" x2="-25.4" y2="-20.32" width="0.254" layer="94"/>
<text x="-5.08" y="1.27" size="1.778" layer="95" font="vector">&gt;NAME</text>
<text x="-5.08" y="-2.54" size="1.778" layer="96" font="vector">&gt;VALUE</text>
<pin name="!MCLR" x="-30.48" y="15.24" length="middle"/>
<pin name="RA0" x="-30.48" y="12.7" length="middle"/>
<pin name="RA1" x="-30.48" y="10.16" length="middle"/>
<pin name="PGED1/RB0" x="-30.48" y="7.62" length="middle"/>
<pin name="PGEC1/RB1" x="-30.48" y="5.08" length="middle"/>
<pin name="RB2" x="-30.48" y="2.54" length="middle"/>
<pin name="RB3" x="-30.48" y="0" length="middle"/>
<pin name="VSS@1" x="-30.48" y="-2.54" length="middle" direction="pwr"/>
<pin name="OSCI/RA2" x="-30.48" y="-5.08" length="middle"/>
<pin name="OSCO/RA3" x="-30.48" y="-7.62" length="middle"/>
<pin name="RB4" x="-30.48" y="-10.16" length="middle"/>
<pin name="RA4" x="-30.48" y="-12.7" length="middle"/>
<pin name="VDD@1" x="-30.48" y="-15.24" length="middle" direction="pwr"/>
<pin name="USBID/RB5" x="-30.48" y="-17.78" length="middle"/>
<pin name="VBUS" x="30.48" y="-17.78" length="middle" rot="R180"/>
<pin name="RP7" x="30.48" y="-15.24" length="middle" rot="R180"/>
<pin name="RP8" x="30.48" y="-12.7" length="middle" direction="in" rot="R180"/>
<pin name="RB9" x="30.48" y="-10.16" length="middle" direction="out" rot="R180"/>
<pin name="DISVREG" x="30.48" y="-7.62" length="middle" direction="pas" rot="R180"/>
<pin name="VCAP/VDDCORE" x="30.48" y="-5.08" length="middle" direction="pwr" rot="R180"/>
<pin name="D+" x="30.48" y="-2.54" length="middle" direction="in" rot="R180"/>
<pin name="D-" x="30.48" y="0" length="middle" direction="in" rot="R180"/>
<pin name="VUSB" x="30.48" y="2.54" length="middle" rot="R180"/>
<pin name="AN11/RB13" x="30.48" y="5.08" length="middle" rot="R180"/>
<pin name="AN10/RB14" x="30.48" y="7.62" length="middle" rot="R180"/>
<pin name="RB15" x="30.48" y="10.16" length="middle" rot="R180"/>
<pin name="VSS@2" x="30.48" y="12.7" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD@2" x="30.48" y="15.24" length="middle" direction="pwr" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PIC24FJ64GB002" prefix="IC">
<description>&lt;b&gt;PIC24FJ64GB002&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="PIC24FJ64GB002" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DIL28-3">
<connects>
<connect gate="G$1" pin="!MCLR" pad="1"/>
<connect gate="G$1" pin="AN10/RB14" pad="25"/>
<connect gate="G$1" pin="AN11/RB13" pad="24"/>
<connect gate="G$1" pin="D+" pad="21"/>
<connect gate="G$1" pin="D-" pad="22"/>
<connect gate="G$1" pin="DISVREG" pad="19"/>
<connect gate="G$1" pin="OSCI/RA2" pad="9"/>
<connect gate="G$1" pin="OSCO/RA3" pad="10"/>
<connect gate="G$1" pin="PGEC1/RB1" pad="5"/>
<connect gate="G$1" pin="PGED1/RB0" pad="4"/>
<connect gate="G$1" pin="RA0" pad="2"/>
<connect gate="G$1" pin="RA1" pad="3"/>
<connect gate="G$1" pin="RA4" pad="12"/>
<connect gate="G$1" pin="RB15" pad="26"/>
<connect gate="G$1" pin="RB2" pad="6"/>
<connect gate="G$1" pin="RB3" pad="7"/>
<connect gate="G$1" pin="RB4" pad="11"/>
<connect gate="G$1" pin="RB9" pad="18"/>
<connect gate="G$1" pin="RP7" pad="16"/>
<connect gate="G$1" pin="RP8" pad="17"/>
<connect gate="G$1" pin="USBID/RB5" pad="14"/>
<connect gate="G$1" pin="VBUS" pad="15"/>
<connect gate="G$1" pin="VCAP/VDDCORE" pad="20"/>
<connect gate="G$1" pin="VDD@1" pad="13"/>
<connect gate="G$1" pin="VDD@2" pad="28"/>
<connect gate="G$1" pin="VSS@1" pad="8"/>
<connect gate="G$1" pin="VSS@2" pad="27"/>
<connect gate="G$1" pin="VUSB" pad="23"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="PIC24FJ64GB002-I/SP" constant="no"/>
<attribute name="OC_FARNELL" value="1332298" constant="no"/>
<attribute name="OC_NEWARK" value="88K6132" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="IC1" library="HHK-Bluetooth" deviceset="PIC24FJ64GB002" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="IC1" gate="G$1" x="33.02" y="50.8"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
