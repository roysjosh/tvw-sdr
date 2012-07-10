<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/pdml/packet">
<!--
  <xsl:value-of select="proto[@name='geninfo']/field[@name='num']/@show" />
  <xsl:text> </xsl:text>
-->
  <xsl:apply-templates select="proto[@name='usb']"/>
  <xsl:text>&#x000a;</xsl:text>
</xsl:template>

<xsl:template match="field[@name='usb.endpoint_number']">
<!--
  <xsl:value-of select="@show"/>
-->
  <xsl:choose>
    <xsl:when test="@show = '0x03'">
      <xsl:text>W | </xsl:text> <!-- write -->
    </xsl:when>
    <xsl:when test="@show = '0x82'">
      <xsl:text>R | </xsl:text> <!-- read -->
    </xsl:when>
    <xsl:otherwise>
      <xsl:text>? | </xsl:text>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!--
<xsl:template match="field[@name='usb.endpoint_number']">
  <xsl:value-of select="@show"/>
  <xsl:text> | </xsl:text>
</xsl:template>
-->

<xsl:template match="field[@name='usb.capdata']">
  <xsl:value-of select="@show"/>
</xsl:template>

</xsl:transform>
