-- 创建数据库
CREATE DATABASE IF NOT EXISTS szl_postgradual_courses;

-- 选择要使用的数据库
USE szl_postgradual_courses;

-- 创建表
CREATE TABLE IF NOT EXISTS course (
    id INT PRIMARY KEY AUTO_INCREMENT,
    course_name VARCHAR(255) NOT NULL,
    instructor VARCHAR(255) NOT NULL,
    score DECIMAL(4, 2) NOT NULL
);

-- 插入数据
INSERT INTO course (course_name, instructor, score) VALUES
    ('Advanced Programming', 'Professor Smith', 90.5),
    ('Database Management', 'Professor Johnson', 85.0),
    ('Machine Learning', 'Professor Williams', 92.3),
    ('Software Engineering', 'Professor Davis', 88.7),
    ('Data Structures', 'Professor Brown', 91.1);
