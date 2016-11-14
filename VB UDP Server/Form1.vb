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
	Public receivingUdpClient As UdpClient
	Public RemoteIpEndPoint As New System.Net.IPEndPoint(System.Net.IPAddress.Any, 0)
	Public ThreadReceive As System.Threading.Thread
	Dim SocketNO As Integer


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
	Friend WithEvents TextBox1 As System.Windows.Forms.TextBox
	Friend WithEvents Button_Clear As System.Windows.Forms.Button
	Friend WithEvents txtSocket As System.Windows.Forms.TextBox
	Friend WithEvents Label1 As System.Windows.Forms.Label
	Friend WithEvents txtIP As System.Windows.Forms.TextBox
	Friend WithEvents Label2 As System.Windows.Forms.Label
	Friend WithEvents Button_Start As System.Windows.Forms.Button
	Friend WithEvents Label3 As System.Windows.Forms.Label
	Friend WithEvents chkBit As System.Windows.Forms.CheckBox
	Friend WithEvents Button_Stop As System.Windows.Forms.Button
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
		Me.TextBox1 = New System.Windows.Forms.TextBox()
		Me.Button_Clear = New System.Windows.Forms.Button()
		Me.txtSocket = New System.Windows.Forms.TextBox()
		Me.Label1 = New System.Windows.Forms.Label()
		Me.txtIP = New System.Windows.Forms.TextBox()
		Me.Label2 = New System.Windows.Forms.Label()
		Me.Button_Start = New System.Windows.Forms.Button()
		Me.Label3 = New System.Windows.Forms.Label()
		Me.chkBit = New System.Windows.Forms.CheckBox()
		Me.Button_Stop = New System.Windows.Forms.Button()
		Me.SuspendLayout()
		'
		'TextBox1
		'
		Me.TextBox1.Enabled = False
		Me.TextBox1.Location = New System.Drawing.Point(0, 32)
		Me.TextBox1.Multiline = True
		Me.TextBox1.Name = "TextBox1"
		Me.TextBox1.ReadOnly = True
		Me.TextBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
		Me.TextBox1.Size = New System.Drawing.Size(712, 352)
		Me.TextBox1.TabIndex = 0
		Me.TextBox1.Text = "INFORMATION"
		'
		'Button_Clear
		'
		Me.Button_Clear.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Button_Clear.Location = New System.Drawing.Point(0, 382)
		Me.Button_Clear.Name = "Button_Clear"
		Me.Button_Clear.Size = New System.Drawing.Size(712, 32)
		Me.Button_Clear.TabIndex = 1
		Me.Button_Clear.Text = "Clear All"
		'
		'txtSocket
		'
		Me.txtSocket.Location = New System.Drawing.Point(136, 6)
		Me.txtSocket.Name = "txtSocket"
		Me.txtSocket.Size = New System.Drawing.Size(80, 20)
		Me.txtSocket.TabIndex = 2
		Me.txtSocket.Text = "11000"
		'
		'Label1
		'
		Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label1.Location = New System.Drawing.Point(0, 8)
		Me.Label1.Name = "Label1"
		Me.Label1.Size = New System.Drawing.Size(128, 16)
		Me.Label1.TabIndex = 3
		Me.Label1.Text = "Receiving Socket No"
		'
		'txtIP
		'
		Me.txtIP.Location = New System.Drawing.Point(496, 6)
		Me.txtIP.Name = "txtIP"
		Me.txtIP.ReadOnly = True
		Me.txtIP.Size = New System.Drawing.Size(96, 20)
		Me.txtIP.TabIndex = 5
		'
		'Label2
		'
		Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label2.Location = New System.Drawing.Point(352, 8)
		Me.Label2.Name = "Label2"
		Me.Label2.Size = New System.Drawing.Size(128, 16)
		Me.Label2.TabIndex = 6
		Me.Label2.Text = "Receiving IP Address"
		'
		'Button_Start
		'
		Me.Button_Start.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Button_Start.Location = New System.Drawing.Point(224, 4)
		Me.Button_Start.Name = "Button_Start"
		Me.Button_Start.Size = New System.Drawing.Size(48, 24)
		Me.Button_Start.TabIndex = 7
		Me.Button_Start.Text = "Start"
		'
		'Label3
		'
		Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 7.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label3.Location = New System.Drawing.Point(488, 416)
		Me.Label3.Name = "Label3"
		Me.Label3.Size = New System.Drawing.Size(224, 16)
		Me.Label3.TabIndex = 8
		Me.Label3.Text = "Created by Kumudu Wickramasinha Gunasekara"
		'
		'chkBit
		'
		Me.chkBit.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.chkBit.Location = New System.Drawing.Point(608, 8)
		Me.chkBit.Name = "chkBit"
		Me.chkBit.Size = New System.Drawing.Size(88, 16)
		Me.chkBit.TabIndex = 9
		Me.chkBit.Text = "Bit Detail"
		'
		'Button_Stop
		'
		Me.Button_Stop.Enabled = False
		Me.Button_Stop.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Button_Stop.Location = New System.Drawing.Point(288, 4)
		Me.Button_Stop.Name = "Button_Stop"
		Me.Button_Stop.Size = New System.Drawing.Size(48, 24)
		Me.Button_Stop.TabIndex = 10
		Me.Button_Stop.Text = "Stop"
		'
		'Form1
		'
		Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
		Me.ClientSize = New System.Drawing.Size(712, 430)
		Me.Controls.Add(Me.Button_Stop)
		Me.Controls.Add(Me.chkBit)
		Me.Controls.Add(Me.Label3)
		Me.Controls.Add(Me.Button_Start)
		Me.Controls.Add(Me.Label2)
		Me.Controls.Add(Me.txtIP)
		Me.Controls.Add(Me.Label1)
		Me.Controls.Add(Me.txtSocket)
		Me.Controls.Add(Me.Button_Clear)
		Me.Controls.Add(Me.TextBox1)
		Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
		Me.Name = "Form1"
		Me.Text = "UDP Receive"
		Me.ResumeLayout(False)
		Me.PerformLayout()

	End Sub

#End Region

	Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		chkBit.Checked = True
	End Sub


	Public Sub ReceiveMessages()
		Try
			Dim receiveBytes As [Byte]() = receivingUdpClient.Receive(RemoteIpEndPoint)
			txtIP.Text = RemoteIpEndPoint.Address.ToString
			Dim BitDet As BitArray
			BitDet = New BitArray(receiveBytes)

			Dim strReturnData As String = System.Text.Encoding.Unicode.GetString(receiveBytes)
			Console.WriteLine("A message is received and being processed")
			TextBox1.Text = TextBox1.Text & vbCrLf & "A message is received and being processed"
			Console.WriteLine("The length of the message is ")
			TextBox1.Text = TextBox1.Text & vbCrLf & "The length of the message is "
			Console.WriteLine(receiveBytes.Length)
			TextBox1.Text = TextBox1.Text & receiveBytes.Length
			Console.WriteLine(strReturnData)
			TextBox1.Text = TextBox1.Text + vbCrLf + "The message received is """
			TextBox1.Text = TextBox1.Text & Encoding.ASCII.GetChars(receiveBytes) + """"
			TextBox1.Text = TextBox1.Text & vbCrLf
			Dim tempStr As String = ""
			Dim tempStr2 As String

			If chkBit.Checked = True Then
				Dim i As Integer
				i = 0
				Dim j As Integer
				j = 0
				Dim line As Integer
				line = 0
				TextBox1.Text = TextBox1.Text + line.ToString & ") "
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
						TextBox1.Text = TextBox1.Text + tempStr
						tempStr = ""
						tempStr2 = ""
						TextBox1.Text = TextBox1.Text + vbCrLf
						If j <> (BitDet.Length - 1) Then
							TextBox1.Text = TextBox1.Text + line.ToString & ") "
							Console.WriteLine()
						End If
					End If
				Next
			End If
			TextBox1.Text = TextBox1.Text & vbCrLf
			NewInitialize()
		Catch e As Exception
			Console.WriteLine(e.Message)
		End Try
	End Sub


	Public Sub NewInitialize()
		Console.WriteLine("Thread *Thread Receive* reinitialized")
		ThreadReceive = New System.Threading.Thread(AddressOf ReceiveMessages)
		ThreadReceive.Start()
	End Sub


	Private Sub Button_Clear_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Clear.Click
		TextBox1.Text = "INFORMATION"
	End Sub


	Private Sub Button_Start_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Start.Click
		Try
			SocketNO = txtSocket.Text
			receivingUdpClient = New System.Net.Sockets.UdpClient(SocketNO)
			ThreadReceive = New System.Threading.Thread(AddressOf ReceiveMessages)
			ThreadReceive.Start()
			TextBox1.Enabled = True
			Button_Stop.Enabled = True
			Button_Start.Enabled = False
			txtSocket.ReadOnly = True
		Catch x As Exception
			Console.WriteLine(x.Message)
			TextBox1.Text = TextBox1.Text & vbCrLf & x.Message
		End Try
	End Sub


	Private Sub Form1_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
		Try
			receivingUdpClient.Close()
		Catch ex As Exception
			Console.WriteLine(ex.Message)
		End Try
	End Sub


	Private Sub Button_Stop_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Stop.Click
		Try
			ThreadReceive.Abort()
			receivingUdpClient.Close()
			TextBox1.Text = "INFORMATION"
			TextBox1.Enabled = False
			Button_Stop.Enabled = False
			Button_Start.Enabled = True
			txtIP.Text = ""
			txtSocket.ReadOnly = False
		Catch ex As Exception
			Console.WriteLine(ex.Message)
		End Try
	End Sub


End Class
