<%@ page language="java" import="java.util.*" pageEncoding="GB18030"%>

   <%@taglib prefix="s" uri="/struts-tags" %>	
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    
    
    <title>quququ</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
	<script type="text/javascript">
		function sub(){
			if(confirm("ȷ��ɾ��")){
				window.location.href='download.action';
			}
		}
	</script>
  </head>
  
  <body>
    !!!!<br>
  		<input type="button" id="receive" value="����" onclick="window.location.href='FirstPage.jsp'" />
		<input type="button" id="send" value="����" onclick="window.location.href='FirstPage.jsp'" />
	    <br><br><br>
	    <br><br>
	    <input type="button" id="browsetags" value="���ҩƷ��Ϣ" onclick="window.location.href='browse_tag.action'">
		 <input type="button" id="browsefactory" value="�����ҩ����Ϣ" onclick="window.location.href='browse_factory.action'">
		 <input type="button" id="browsestore" value="����̵���Ϣ" onclick="window.location.href='browse_store.action'">
		  <input type="button" id="browsetransport" value="������乫˾��Ϣ" onclick="window.location.href='browse_transport.action'">
		
		<table border="1" align = "center">
    		<tr>
	    		<td >�˴�����������</td>
	    		<td>������������͵</td>
	    		<br>
	    		<tr>
		    		<td text="disk" >
					</td>
					<td text="disk" >
					</td>
				<tr>
				
   			</tr>
    	</table>
  </body>
</html>
