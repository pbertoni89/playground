Public Class Form_Host

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        Dim f As New Form_Guest()
        f.TopLevel = False
        f.WindowState = FormWindowState.Maximized
        f.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        f.Visible = True
        Panel1.Controls.Add(f)

        Dim f2 As New FormDll.Form_Guest_Dll
        f2.TopLevel = False
        f2.WindowState = FormWindowState.Maximized
        f2.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        f2.Visible = True
        Panel2.Controls.Add(f2)

    End Sub

End Class
