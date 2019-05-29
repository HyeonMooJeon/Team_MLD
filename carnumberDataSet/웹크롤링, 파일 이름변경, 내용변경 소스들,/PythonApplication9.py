from icrawler.builtin import GoogleImageCrawler
 
google_crawler = GoogleImageCrawler(parser_threads=2, downloader_threads=4,
                                    storage={'root_dir': 'your_image_dir2'})
google_crawler.crawl(keyword='번호판', max_num=1000,
                   
                     min_size=(200,200), max_size=None)