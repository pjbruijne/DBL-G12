package Hardware;
public class Disk {

    final private int color; // 0 = white, 1 = black
    final private float diameter = 2.3f; // in cm
    final private float height = 1f; // in cm

    public Disk(int color) {
        this.color = color;
    }

    public int getColor() {
        return color;
    }

    public float getDiameter() {
        return diameter;
    }

    public float getHeight() {
        return height;
    }
}
