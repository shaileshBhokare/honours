package org.smarteda.web;
import org.springframework.web.servlet.mvc.Controller;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import java.io.IOException;
import java.util.Map;
import java.util.HashMap;

import org.smarteda.bus.Product;
import org.smarteda.bus.ProductManager;

public class SpringappController implements Controller {

	private ProductManager productManager;
	
    public ModelAndView handleRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
    	
    	String now = (new java.util.Date()).toString();
    	
    	Map myModel = new HashMap();
    	myModel.put("now", now);
    	myModel.put("products", getProductManager().getProducts());
    	
        return new ModelAndView("hello", "model", myModel);
    }

	public ProductManager getProductManager() {
		return productManager;
	}

	public void setProductManager(ProductManager productManager) {
		this.productManager = productManager;
	}
}