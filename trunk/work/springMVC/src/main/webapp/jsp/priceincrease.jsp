<%@ include file="/jsp/include.jsp" %>
<%@ taglib prefix="spring" uri="/spring" %>

<html>
<head><title><fmt:message key="title"/></title></head>
<body>
<h1><fmt:message key="priceincrease.heading"/></h1>
<form method="post">
  <table width="95%" bgcolor="f8f8ff" border="0" cellspacing="0" cellpadding="5">
    <tr>
      <td alignment="right" width="20%">Increase (%):</td>
      <spring:bind path="priceIncrease.percentage">
        <td width="20%">
          <input type="text" name="percentage" value="<c:out value="${status.value}"/>">
        </td>
        <td width="60%">
          <font color="red"><c:out value="${status.errorMessage}"/></font>
        </td>
      </spring:bind>
    </tr>
  </table>
  <br>
  <spring:hasBindErrors name="priceIncrease">
    <b>Please fix all errors!</b>
  </spring:hasBindErrors>
  <br><br>
  <input type="submit" alignment="center" value="Execute">
</form>
<a href="<c:url value="hello.htm"/>">Home</a>
</body>
</html>