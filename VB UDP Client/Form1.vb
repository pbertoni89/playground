Imports System.IO
Imports System.Net.Sockets.Socket
Imports System
Imports System.Text
Imports System.Net
Imports System.Net.Sockets
Imports Microsoft.VisualBasic
Imports System.Runtime.InteropServices
Public Class Form1
    Inherits System.Windows.Forms.Form
    Dim GLOIP As IPAddress
    Dim GLOINTPORT As Integer
    Dim bytCommand As Byte() = New Byte() {}
    Dim udpClient As New UdpClient
#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents cmdSend As System.Windows.Forms.Button
    Friend WithEvents txtInfo As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents txtIP As System.Windows.Forms.TextBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents txtPort As System.Windows.Forms.TextBox
    Friend WithEvents txtMessage As System.Windows.Forms.TextBox
    Friend WithEvents Label4 As System.Windows.Forms.Label
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(Form1))
        Me.cmdSend = New System.Windows.Forms.Button
        Me.txtInfo = New System.Windows.Forms.TextBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.txtMessage = New System.Windows.Forms.TextBox
        Me.Label2 = New System.Windows.Forms.Label
        Me.txtIP = New System.Windows.Forms.TextBox
        Me.Label3 = New System.Windows.Forms.Label
        Me.txtPort = New System.Windows.Forms.TextBox
        Me.Label4 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'cmdSend
        '
        Me.cmdSend.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdSend.Location = New System.Drawing.Point(392, 36)
        Me.cmdSend.Name = "cmdSend"
        Me.cmdSend.Size = New System.Drawing.Size(96, 24)
        Me.cmdSend.TabIndex = 0
        Me.cmdSend.Text = "Send"
        '
        'txtInfo
        '
        Me.txtInfo.Location = New System.Drawing.Point(8, 80)
        Me.txtInfo.Multiline = True
        Me.txtInfo.Name = "txtInfo"
        Me.txtInfo.ReadOnly = True
        Me.txtInfo.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.txtInfo.Size = New System.Drawing.Size(640, 304)
        Me.txtInfo.TabIndex = 1
        Me.txtInfo.Text = "INFORMATION"
        '
        'Label1
        '
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(16, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(96, 16)
        Me.Label1.TabIndex = 2
        Me.Label1.Text = "Data To Send"
        Me.Label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'txtMessage
        '
        Me.txtMessage.Location = New System.Drawing.Point(120, 6)
        Me.txtMessage.Multiline = True
        Me.txtMessage.Name = "txtMessage"
        Me.txtMessage.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.txtMessage.Size = New System.Drawing.Size(520, 20)
        Me.txtMessage.TabIndex = 3
        Me.txtMessage.Text = "TEST"
        '
        'Label2
        '
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(16, 40)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(96, 16)
        Me.Label2.TabIndex = 4
        Me.Label2.Text = "IP Address"
        Me.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'txtIP
        '
        Me.txtIP.Location = New System.Drawing.Point(120, 38)
        Me.txtIP.Name = "txtIP"
        Me.txtIP.Size = New System.Drawing.Size(88, 20)
        Me.txtIP.TabIndex = 5
        Me.txtIP.Text = "000.000.000.000"
        '
        'Label3
        '
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(216, 40)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(64, 16)
        Me.Label3.TabIndex = 6
        Me.Label3.Text = "Port"
        Me.Label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'txtPort
        '
        Me.txtPort.Location = New System.Drawing.Point(288, 38)
        Me.txtPort.Name = "txtPort"
        Me.txtPort.Size = New System.Drawing.Size(88, 20)
        Me.txtPort.TabIndex = 7
        Me.txtPort.Text = "11000"
        '
        'Label4
        '
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 7.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(432, 384)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(224, 16)
        Me.Label4.TabIndex = 8
        Me.Label4.Text = "Created by Kumudu Wickramasinha Gunasekara"
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(656, 398)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.txtPort)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.txtIP)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.txtMessage)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.txtInfo)
        Me.Controls.Add(Me.cmdSend)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "Form1"
        Me.Text = "UDP Send"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub cmdSend_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdSend.Click

        Dim pRet As Integer

        Try
            GLOIP = IPAddress.Parse(txtIP.Text)
            GLOINTPORT = txtPort.Text
            udpClient.Connect(GLOIP, GLOINTPORT)
            bytCommand = Encoding.ASCII.GetBytes(txtMessage.Text)
            pRet = udpClient.Send(bytCommand, bytCommand.Length)
            Console.WriteLine("No of bytes send " & pRet)
            txtInfo.Text = "INFORMATION" & vbCrLf & "No of bytes send " & pRet
            Console.WriteLine(Encoding.ASCII.GetString(bytCommand))
            txtInfo.Text = txtInfo.Text + vbCrLf + "The messege send is """
            txtInfo.Text = txtInfo.Text & Encoding.ASCII.GetChars(bytCommand) & """"
            Dim BitDet As BitArray
            BitDet = New BitArray(bytCommand)
            txtInfo.Text = txtInfo.Text + vbCrLf
            Dim tempStr As String
            Dim tempStr2 As String
            Dim i As Integer
            i = 0
            Dim j As Integer
            j = 0
            Dim line As Integer
            line = 0
            txtInfo.Text = txtInfo.Text + line.ToString & ") "
            For j = 0 To BitDet.Length - 1
                If BitDet(j) = True Then
                    Console.Write("1 ")
                    tempStr2 = tempStr
                    tempStr = " 1" + tempStr2
                Else
                    Console.Write("0 ")
                    tempStr2 = tempStr
                    tempStr = " 0" + tempStr2
                End If
                i += 1
                If i = 8 And j <= (BitDet.Length - 1) Then
                    line += 1
                    i = 0
                    txtInfo.Text = txtInfo.Text + tempStr
                    tempStr = ""
                    tempStr2 = ""
                    txtInfo.Text = txtInfo.Text + vbCrLf
                    If j <> (BitDet.Length - 1) Then
                        txtInfo.Text = txtInfo.Text + line.ToString & ") "
                        Console.WriteLine()
                    End If
                End If
            Next




        Catch ex As Exception
            Console.WriteLine(ex.Message)
            txtInfo.Text = txtInfo.Text & vbCrLf & ex.Message
        End Try
    End Sub

  

    Private Sub Form1_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        udpClient.Close()
    End Sub
End Class
