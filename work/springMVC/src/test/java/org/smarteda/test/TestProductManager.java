package org.smarteda.test;

import java.util.List;
import java.util.ArrayList;
import junit.framework.TestCase;
import org.smarteda.bus.ProductManager;
import org.smarteda.bus.Product;

public class TestProductManager extends TestCase {

    private ProductManager pm;

    public void setUp() {
        pm = new ProductManager();
        Product p = new Product();
        p.setDescription("Chair");
        p.setPrice(new Double("20.50"));
        ArrayList al = new ArrayList();
        al.add(p);
        p = new Product();
        p.setDescription("Table");
        p.setPrice(new Double("150.10"));
        al.add(p);
        pm.setProducts(al);
    }

    public void testGetProducs() {
        List l = pm.getProducts();
        Product p1 = (Product) l.get(0);
        assertEquals("Chair", p1.getDescription());
        Product p2 = (Product) l.get(1);
        assertEquals("Table", p2.getDescription());
    }

    public void testIncreasePrice() {
        pm.increasePrice(10);
        List l = pm.getProducts();
        Product p = (Product) l.get(0);
        assertEquals(new Double("22.55"), p.getPrice());
        p = (Product) l.get(1);
        assertEquals(new Double("165.11"), p.getPrice());
    }

}