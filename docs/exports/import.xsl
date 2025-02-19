<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

    <xsl:variable name="baseurl">
        <xsl:text>../</xsl:text>
    </xsl:variable>

    <xsl:template match="xml">
        <html>
            <head>
                <title><xsl:value-of select="title"/></title>
            </head>
            <body topmargin="0" rightmargin="0" leftmargin="0" bottommargin="0">
                <xsl:attribute name="background">
                    <xsl:value-of select="$baseurl"/>
                    <xsl:text>utgs-bg.jpg</xsl:text>
                </xsl:attribute>

                <table border="0" cellspacing="0" cellpadding="0" width="100%" height="64" bgcolor="#3c8fdc">
                    <xsl:attribute name="background">
                        <xsl:value-of select="$baseurl"/>
                        <xsl:text>utgslogobg.jpg</xsl:text>
                    </xsl:attribute>
                    <tr valign="top">
                        <td width="128px">
                            <table width="128px"/>
                        </td>
                        <td valign="middle" width="99%">
                            <xsl:element name="img">
                                <xsl:attribute name="src">
                                    <xsl:value-of select="$baseurl"/>
                                    <xsl:text>utgslogo.png</xsl:text>
                                </xsl:attribute>
                            </xsl:element>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" bgcolor="black">
                            <table height="2px"/>
                        </td>
                    </tr>
                </table>

                <table width="80%" align="center" cellspacing="0" cellpadding="0">
                    <tr height="10px"/>
                    <tr height="2px">
                        <td bgcolor="black"/>
                    </tr>
                    <tr>
                        <td>
                            <table cellpadding="10" width="100%">
                                <xsl:attribute name="background">
                                    <xsl:value-of select="$baseurl"/>
                                    <xsl:text>utgs-bg2.jpg</xsl:text>
                                </xsl:attribute>
                                <tr>
                                    <td>
                                        <h2><xsl:value-of select="title"/></h2>
                                        <h4><xsl:value-of select="description"/></h4>
                                    </td>
                                </tr>
                                <tr>
                                    <td>
                                        <a href="index.html">The List Of Built-In's</a>
                                        <h4>Signals:</h4>
                                        <ul>
                                            <xsl:for-each select="block/signal">
                                                <li>
                                                    <a>
                                                        <xsl:attribute name="href">
                                                            <xsl:text>#signal_</xsl:text>
                                                            <xsl:value-of select="@id"/>
                                                        </xsl:attribute>
                                                        <xsl:value-of select="@id"/>()
                                                    </a>
                                                </li>
                                            </xsl:for-each>
                                        </ul>
                                        <h4>Methods:</h4>
                                        <ul>
                                            <xsl:for-each select="block/chunk">
                                                <li>
                                                    <a>
                                                        <xsl:attribute name="href">
                                                            <xsl:text>#chunk_</xsl:text>
                                                            <xsl:value-of select="@id"/>
                                                        </xsl:attribute>
                                                        <xsl:value-of select="@id"/>()
                                                    </a>
                                                </li>
                                            </xsl:for-each>
                                        </ul>
                                    </td>
                                </tr>
                                <tr height="64px"/>
                                <tr>
                                    <td align="right" valign="bottom" width="99%">
                                        See <a href="http://utgs.berlios.de">UTGS Home Page</a>.<br/>
                                        See <a href="../COPYING">COPYING</a> for license information.<br/>
                                        Copyright (c) 2005, Sadhbh Code (<a href="https://github.com/sadhbh-c0d3">https://github.com/sadhbh-c0d3</a>) All rights reserved.<br/>
                                    </td>
                                </tr>
                                <tr height="64px"/>
                                <tr>
                                    <td>
                                        <xsl:apply-templates/>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr height="2px">
                        <td bgcolor="black"/>
                    </tr>
                </table>
                <table>
                    <tr height="480px"/>
                </table>


            </body>
        </html>
    </xsl:template>

    <xsl:template match="*">
    </xsl:template>

    <xsl:template name="do_block" match="block">
        <xsl:if test="signal">
            <h2>Signals:</h2>
            <hr/>
            <blockquote>
                <xsl:for-each select="signal">
                    <div>
                        <xsl:attribute name="id">
                            <xsl:text>signal_</xsl:text>
                            <xsl:value-of select="@id"/>
                        </xsl:attribute>
                        <h2>
                            <xsl:value-of select="@id"/>
                        </h2>
                        <h4>
                            <code>
                                <xsl:value-of select="./type-name"/>
                            </code>
                        </h4>
                        <code>
                            <xsl:apply-templates select="./description"/>
                        </code>
                    </div>
                    <table height="20pt"/>
                </xsl:for-each>
            </blockquote>
        </xsl:if>
        <xsl:if test="chunk">
            <h2>Member Functions:</h2>
            <hr/>
            <blockquote>
                <xsl:for-each select="chunk">
                    <div>
                        <xsl:attribute name="id">
                            <xsl:text>chunk_</xsl:text>
                            <xsl:value-of select="@id"/>
                        </xsl:attribute>
                        <h2>
                            <xsl:value-of select="@id"/>
                        </h2>
                        <h4>
                            <code>
                                <xsl:value-of select="./type-name"/>
                            </code>
                        </h4>
                        <code>
                            <xsl:apply-templates select="./description"/>
                        </code>
                    </div>
                    <table height="20pt"/>
                </xsl:for-each>
            </blockquote>
        </xsl:if>
        <xsl:if test="string">
            <h2>Constant Strings:</h2>
            <hr/>
            <blockquote>
                <xsl:for-each select="string">
                    <div>
                        <xsl:attribute name="id">
                            <xsl:text>string_</xsl:text>
                            <xsl:value-of select="@id"/>
                        </xsl:attribute>
                        <h2>
                            <xsl:value-of select="@id"/>
                        </h2>
                        <h4>
                            <code>unicode-string</code> (C++ class TextUTF8)
                        </h4>
                        <code>
                            <xsl:value-of select="./description"/>
                        </code>
                    </div>
                    <table height="20pt"/>
                </xsl:for-each>
            </blockquote>
        </xsl:if>
        <xsl:if test="integer">
            <h2>Constant Integer Values:</h2>
            <hr/>
            <blockquote>
                <xsl:for-each select="integer">
                    <div>
                        <xsl:attribute name="id">
                            <xsl:text>integer_</xsl:text>
                            <xsl:value-of select="@id"/>
                        </xsl:attribute>
                        <h2>
                            <xsl:value-of select="@id"/>
                        </h2>
                        <h4>
                            <code>integer</code> (C++ int)
                        </h4>
                        <code>
                            <xsl:value-of select="./description"/>
                        </code>
                    </div>
                    <table height="20pt"/>
                </xsl:for-each>
            </blockquote>
        </xsl:if>
        <xsl:if test="float">
            <h2>Constant Floating Point Numbers:</h2>
            <hr/>
            <blockquote>
                <xsl:for-each select="float">
                    <div>
                        <xsl:attribute name="id">
                            <xsl:text>float_</xsl:text>
                            <xsl:value-of select="@id"/>
                        </xsl:attribute>
                        <h2>
                            <xsl:value-of select="@id"/>
                        </h2>
                        <h4>
                            <code>float</code> (C++ double)
                        </h4>
                        <code>
                            <xsl:value-of select="./description"/>
                        </code>
                    </div>
                    <table height="20pt"/>
                </xsl:for-each>
            </blockquote>
        </xsl:if>
        <xsl:if test="block">
            <h2>Constant Blocks:</h2>
            <hr/>
            <blockquote>
                <xsl:for-each select="block">
                    <h2>
                        <xsl:value-of select="@id"/>
                    </h2>
                    <h4>
                        <code>block</code> (C++ IBlock)
                    </h4>
                    <code>
                        <xsl:value-of select="./description"/>
                    </code>
                    <font size="-1">
                        <!-- xsl:call-template name="do_block" select="block"/ -->
                        <table height="20pt"/>
                    </font>
                </xsl:for-each>
            </blockquote>
        </xsl:if>
    </xsl:template>
    
    <xsl:template match="description">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="a">
        <b>
            <font color="darkmagenta">
                <xsl:apply-templates/>
            </font>
        </b>
    </xsl:template>
    
    <xsl:template match="p">
        <p>
            <xsl:apply-templates/>
        </p>
    </xsl:template>
    
    <xsl:template match="note">
        <p>
            <b>Note: </b><xsl:apply-templates/>
        </p>
    </xsl:template>
    
    <xsl:template match="warning">
        <p>
            <b>Warning: </b><xsl:apply-templates/>
        </p>
    </xsl:template>
    
    <xsl:template match="pre">
        <pre>
            <xsl:copy select="node()"/>
        </pre>
    </xsl:template>
    
    <xsl:template match="parameters">
        <h4>Parameters: </h4>
        <ul>
            <xsl:apply-templates/>
        </ul>
    </xsl:template>
    
    <xsl:template match="param">
        <li>
            <b>
                <xsl:apply-templates select="n/node()"/>
                <xsl:text>: </xsl:text>
            </b>
            <xsl:apply-templates select="d/node()"/>
        </li>
    </xsl:template>
    
    <xsl:template match="returns">
        <h4>Return Values: </h4>
        <ul>
            <xsl:apply-templates/>
        </ul>
    </xsl:template>
    
    <xsl:template match="ret">
        <li>
            <b>
                <xsl:apply-templates select="n/node()"/>
                <xsl:text>: </xsl:text>
            </b>
            <xsl:apply-templates select="d/node()"/>
        </li>
    </xsl:template>
    
    <xsl:template match="examples">
        <h4>Examples: </h4>
        <xsl:apply-templates/>
    </xsl:template>
    
    <xsl:template match="example">
        <blockquote>
            <xsl:apply-templates/>
        </blockquote>
    </xsl:template>

    <xsl:template match="code[@lang='xml']">
        <xsl:for-each select="*">
            <xsl:call-template name="codePartXML"/>
            <br/>
        </xsl:for-each>
    </xsl:template>
    
    <xsl:template name="codePartXML">
        <code>
            <font color="darkcyan">
                <b>
                    <xsl:text>&lt;</xsl:text>
                    <xsl:value-of select="name()"/>
                </b>
            </font>
            <xsl:if test="@*">
                <xsl:text> </xsl:text>
            </xsl:if>
            <xsl:for-each select="@*">
                <b>
                    <font color="green">
                        <xsl:value-of select="name()"/>
                    </font>
                    <xsl:text>=</xsl:text>
                    <font color="darkmagenta">
                        <xsl:text>"</xsl:text>
                        <xsl:value-of select="."/>
                        <xsl:text>"</xsl:text>
                    </font>
                    <xsl:text> </xsl:text>
                </b>
            </xsl:for-each>
            <xsl:choose>
                <xsl:when test="./*">
                    <font color="darkcyan">
                        <b>
                            <xsl:text>&gt;</xsl:text>
                        </b>
                    </font>
                    <xsl:for-each select="./*">
                        <table cellspacing="4" cellpadding="0">
                            <tr>
                                <td width="16px"/>
                                <td>
                                    <xsl:choose>
                                        <xsl:when test="name() = 'fixed-comment'">
                                            <font color="blue">
                                                <xsl:text>&lt;!-- </xsl:text>
                                                <xmp>
                                                    <xsl:value-of select="."/>
                                                </xmp>
                                                <xsl:text> --&gt;</xsl:text>
                                            </font>
                                        </xsl:when>
                                        <xsl:when test="name() = 'comment'">
                                            <font color="blue">
                                                <xsl:text>&lt;!-- </xsl:text>
                                                <xsl:value-of select="."/>
                                                <xsl:text> --&gt;</xsl:text>
                                            </font>
                                        </xsl:when>
                                        <xsl:otherwise>
                                            <xsl:call-template name="codePartXML"/>
                                        </xsl:otherwise>
                                    </xsl:choose>
                                </td>
                            </tr>
                        </table>
                    </xsl:for-each>
                    <font color="darkcyan">
                        <b>
                            <xsl:text>&lt;/</xsl:text>
                            <xsl:value-of select="name()"/>
                            <xsl:text>&gt;</xsl:text>
                        </b>
                    </font>
                </xsl:when>
                <xsl:when test="node()">
                    <font color="darkcyan">
                        <b>
                            <xsl:text>&gt;</xsl:text>
                        </b>
                    </font>
                    <xsl:value-of select="node()"/>
                    <font color="darkcyan">
                        <b>
                            <xsl:text>&lt;/</xsl:text>
                            <xsl:value-of select="name()"/>
                            <xsl:text>&gt;</xsl:text>
                        </b>
                    </font>
                </xsl:when>
                <xsl:otherwise>
                    <font color="darkcyan">
                        <b>
                            <xsl:text>/&gt;</xsl:text>
                        </b>
                    </font>
                </xsl:otherwise>
            </xsl:choose>
        </code>
    </xsl:template>
    
    <xsl:template name="codePartText" match="code[@lang='text']">
        <code>
            <xsl:apply-templates/>
        </code>
    </xsl:template>
    
    <xsl:template name="codePartCpp" match="code[@lang='cpp']">
        <code>
            <xsl:apply-templates/>
        </code>
    </xsl:template>

    <xsl:template match="code[@lang='cpp']/s">
        <font color="blue">
            <xsl:apply-templates/>
        </font>
    </xsl:template>
    
    <xsl:template match="code[@lang='cpp']/v">
        <font color="magenta">
            <xsl:apply-templates/>
        </font>
    </xsl:template>
    
    <xsl:template match="code[@lang='cpp']/o">
        <font color="black">
            <xsl:apply-templates/>
        </font>
    </xsl:template>

    <xsl:template match="pre">
        <pre>
            <xsl:apply-templates/>
        </pre>
    </xsl:template>


</xsl:stylesheet>
