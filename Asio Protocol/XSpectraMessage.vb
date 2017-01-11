Public Class XSpectraMessage

	Const _HEADER_LENGTH As UInteger = 5
	Const _MAX_BODY_LENGTH As UInteger = 512

	Dim _header(_HEADER_LENGTH - 1) As Char
	Dim _body(_MAX_BODY_LENGTH - 1) As Char

	Dim _body_length As UInteger = 0


	Sub New(ByVal byteArray As IEnumerable(Of Byte))

		Dim tot_length As UInteger = byteArray.Count
		If tot_length < _HEADER_LENGTH Then
			MsgBox("Tot Len = " & tot_length)
		End If

		_body_length = tot_length - _HEADER_LENGTH

		For i As UInteger = 0 To _HEADER_LENGTH - 1

			_header(i) = Convert.ToChar(byteArray(i))

		Next

		For i As UInteger = _HEADER_LENGTH To tot_length - 1

			_body(i - _HEADER_LENGTH) = Convert.ToChar(byteArray(i))

		Next

	End Sub


	Sub New(ByVal message As String)

		If message.Length > _MAX_BODY_LENGTH Then
			Error "message TOO LONG"
		End If

		_body_length = message.Length
		Dim tmpCharArray As Char() = message.ToCharArray()

		For i As UInteger = 0 To _body_length - 1
			_body(i) = tmpCharArray(i)
		Next

		encode_header()

	End Sub

	Public Function body() As Char()

		Return _body

	End Function


	Public Function ToByteArray() As Byte()

		Dim retval(_HEADER_LENGTH + _body_length - 1) As Byte

		For i As UInteger = 0 To _HEADER_LENGTH - 1
			retval(i) = Convert.ToByte(_header(i))
		Next

		For i As UInteger = 0 To _body_length - 1
			retval(i + _HEADER_LENGTH) = Convert.ToByte(_body(i))
		Next

		Return retval

	End Function


	Public Function to_string() As String

		Return New String(_body)

	End Function

	Public Sub set_body_length(body_length As UInteger)
		_body_length = body_length
	End Sub

	Public Function get_body_length() As UInteger
		Return _body_length
	End Function

	' Get _body_length from header
	Public Function decode_header() As Boolean
		_body_length = UInteger.Parse(_header)

		If (_body_length > _MAX_BODY_LENGTH) Then

			MsgBox("Parse strange length " & _body_length & " from " & _header)
			Return False
		End If

		Return True
	End Function

	' We got 4+1 Chars to express the body_length
	Public Sub encode_header()

		_header = _body_length.ToString("D" & _HEADER_LENGTH).ToCharArray()

	End Sub

End Class
