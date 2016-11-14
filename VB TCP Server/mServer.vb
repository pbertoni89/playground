Imports System.Net.Sockets
Imports System.Text
Module mServer

	Sub Main()

		Dim localAddr As Net.IPAddress = Net.IPAddress.Parse("127.0.0.1")
		Dim serverSocket As New TcpListener(localAddr, 8888)

		Dim requestCount As Integer

		Dim clientSocket As TcpClient

		serverSocket.Start()

		msg("Server Started")

		clientSocket = serverSocket.AcceptTcpClient()

		msg("Accept connection from client")

		requestCount = 0

		While (True)

			Try

				requestCount = requestCount + 1

				Dim networkStream As NetworkStream = clientSocket.GetStream()
				Dim bytesFrom(10024) As Byte

				networkStream.Read(bytesFrom, 0, CInt(clientSocket.ReceiveBufferSize))

				Dim dataFromClient As String = System.Text.Encoding.ASCII.GetString(bytesFrom)

				dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("$"))

				msg("Data from client -  " + dataFromClient)

				Dim serverResponse As String = "Server response " + Convert.ToString(requestCount)

				Dim sendBytes As [Byte]() = Encoding.ASCII.GetBytes(serverResponse)

				networkStream.Write(sendBytes, 0, sendBytes.Length)

				networkStream.Flush()

				msg(serverResponse)

			Catch ex As Exception

				MsgBox(ex.ToString)

			End Try

		End While

		clientSocket.Close()

		serverSocket.Stop()

		msg("exit")

		Console.ReadLine()

	End Sub



	Sub msg(ByVal mesg As String)

		mesg.Trim()
		Console.WriteLine(" >> " + mesg)

	End Sub
End Module
